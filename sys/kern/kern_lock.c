/*	$OpenBSD: src/sys/kern/kern_lock.c,v 1.9.4.5 2003/05/15 04:08:02 niklas Exp $	*/

/* 
 * Copyright (c) 1995
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code contains ideas from software contributed to Berkeley by
 * Avadis Tevanian, Jr., Michael Wayne Young, and the Mach Operating
 * System project at Carnegie-Mellon University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)kern_lock.c	8.18 (Berkeley) 5/21/95
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/lock.h>
#include <sys/systm.h>

#include <machine/cpu.h>

void record_stacktrace(int *, int);
void playback_stacktrace(int *, int);

/*
 * Locking primitives implementation.
 * Locks provide shared/exclusive sychronization.
 */

#if 0
#ifdef DEBUG
#define COUNT(p, x) if (p) (p)->p_locks += (x)
#else
#define COUNT(p, x)
#endif
#endif

#define COUNT(p, x)

#if NCPUS > 1

/*
 * For multiprocessor system, try spin lock first.
 *
 * This should be inline expanded below, but we cannot have #if
 * inside a multiline define.
 */
int lock_wait_time = 100;
#define PAUSE(lkp, wanted)						\
		if (lock_wait_time > 0) {				\
			int i;						\
									\
			simple_unlock(&lkp->lk_interlock);		\
			for (i = lock_wait_time; i > 0; i--)		\
				if (!(wanted))				\
					break;				\
			simple_lock(&lkp->lk_interlock);		\
		}							\
		if (!(wanted))						\
			break;

#else /* NCPUS == 1 */

/*
 * It is an error to spin on a uniprocessor as nothing will ever cause
 * the simple lock to clear while we are executing.
 */
#define PAUSE(lkp, wanted)

#endif /* NCPUS == 1 */

/*
 * Acquire a resource.
 */
#define ACQUIRE(lkp, error, extflags, wanted)				\
	PAUSE(lkp, wanted);						\
	for (error = 0; wanted; ) {					\
		(lkp)->lk_waitcount++;					\
		simple_unlock(&(lkp)->lk_interlock);			\
		error = tsleep((void *)lkp, (lkp)->lk_prio,		\
		    (lkp)->lk_wmesg, (lkp)->lk_timo);			\
		simple_lock(&(lkp)->lk_interlock);			\
		(lkp)->lk_waitcount--;					\
		if (error)						\
			break;						\
		if ((extflags) & LK_SLEEPFAIL) {			\
			error = ENOLCK;					\
			break;						\
		}							\
	}

#define	SETHOLDER(lkp, pid, cpu_id)					\
do {									\
	if ((lkp)->lk_flags & LK_SPIN)					\
		(lkp)->lk_cpu = cpu_id;					\
	else {								\
		(lkp)->lk_lockholder = pid;				\
	}								\
} while (/*CONSTCOND*/0)

#define	WEHOLDIT(lkp, pid, cpu_id)					\
	(((lkp)->lk_flags & LK_SPIN) != 0 ?				\
	 ((lkp)->lk_cpu == (cpu_id)) : ((lkp)->lk_lockholder == (pid)))

#define	WAKEUP_WAITER(lkp)						\
do {									\
	if (((lkp)->lk_flags & LK_SPIN) == 0 && (lkp)->lk_waitcount) {	\
		/* XXX Cast away volatile. */				\
		wakeup((void *)(lkp));					\
	}								\
} while (/*CONSTCOND*/0)

/*
 * Initialize a lock; required before use.
 */
void
lockinit(lkp, prio, wmesg, timo, flags)
	struct lock *lkp;
	int prio;
	char *wmesg;
	int timo;
	int flags;
{

	bzero(lkp, sizeof(struct lock));
	simple_lock_init(&lkp->lk_interlock);
	lkp->lk_flags = flags & LK_EXTFLG_MASK;
	lkp->lk_prio = prio;
	lkp->lk_timo = timo;
	lkp->lk_wmesg = wmesg;
	lkp->lk_lockholder = LK_NOPROC;
}

/*
 * Determine the status of a lock.
 */
int
lockstatus(lkp)
	struct lock *lkp;
{
	int lock_type = 0;

	simple_lock(&lkp->lk_interlock);
	if (lkp->lk_exclusivecount != 0)
		lock_type = LK_EXCLUSIVE;
	else if (lkp->lk_sharecount != 0)
		lock_type = LK_SHARED;
	simple_unlock(&lkp->lk_interlock);
	return (lock_type);
}

/*
 * Set, change, or release a lock.
 *
 * Shared requests increment the shared count. Exclusive requests set the
 * LK_WANT_EXCL flag (preventing further shared locks), and wait for already
 * accepted shared locks and shared-to-exclusive upgrades to go away.
 */
int
lockmgr(lkp, flags, interlkp, p)
	__volatile struct lock *lkp;
	u_int flags;
	struct simplelock *interlkp;
	struct proc *p;
{
	int error;
	pid_t pid;
	int extflags;
	cpuid_t cpu_id;

	error = 0;

	simple_lock(&lkp->lk_interlock);
	if (flags & LK_INTERLOCK)
		simple_unlock(interlkp);
	extflags = (flags | lkp->lk_flags) & LK_EXTFLG_MASK;

#ifdef DIAGNOSTIC /* { */
	/*
	 * Don't allow spins on sleep locks and don't allow sleeps
	 * on spin locks.
	 */
	if ((flags ^ lkp->lk_flags) & LK_SPIN)
		panic("lockmgr: sleep/spin mismatch");
#endif /* } */

	if (extflags & LK_SPIN) {
		pid = LK_KERNPROC;
	} else {
		/* XXX Check for p == NULL */
		pid = p->p_pid;
	}
	cpu_id = cpu_number();

	/*
	 * Once a lock has drained, the LK_DRAINING flag is set and an
	 * exclusive lock is returned. The only valid operation thereafter
	 * is a single release of that exclusive lock. This final release
	 * clears the LK_DRAINING flag and sets the LK_DRAINED flag. Any
	 * further requests of any sort will result in a panic. The bits
	 * selected for these two flags are chosen so that they will be set
	 * in memory that is freed (freed memory is filled with 0xdeadbeef).
	 * The final release is permitted to give a new lease on life to
	 * the lock by specifying LK_REENABLE.
	 */
	if (lkp->lk_flags & (LK_DRAINING|LK_DRAINED)) {
#ifdef DIAGNOSTIC
		if (lkp->lk_flags & LK_DRAINED)
			panic("lockmgr: using decommissioned lock");
		if ((flags & LK_TYPE_MASK) != LK_RELEASE ||
		    WEHOLDIT(lkp, pid, cpu_id) == 0)
			panic("lockmgr: non-release on draining lock: %d",
			    flags & LK_TYPE_MASK);
#endif /* DIAGNOSTIC */
		lkp->lk_flags &= ~LK_DRAINING;
		if ((flags & LK_REENABLE) == 0)
			lkp->lk_flags |= LK_DRAINED;
	}

	/*
	 * Check if the caller is asking us to be schizophrenic.
	 */
	if ((lkp->lk_flags & (LK_CANRECURSE|LK_RECURSEFAIL)) ==
	    (LK_CANRECURSE|LK_RECURSEFAIL))
		panic("lockmgr: make up your mind");

	switch (flags & LK_TYPE_MASK) {

	case LK_SHARED:
		if (WEHOLDIT(lkp, pid, cpu_id) == 0) {
			/*
			 * If just polling, check to see if we will block.
			 */
			if ((extflags & LK_NOWAIT) && (lkp->lk_flags &
			    (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE))) {
				error = EBUSY;
				break;
			}
			/*
			 * Wait for exclusive locks and upgrades to clear.
			 */
			ACQUIRE(lkp, error, extflags, lkp->lk_flags &
			    (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE));
			if (error)
				break;
			lkp->lk_sharecount++;
			COUNT(p, 1);
			break;
		}
		/*
		 * We hold an exclusive lock, so downgrade it to shared.
		 * An alternative would be to fail with EDEADLK.
		 */
		lkp->lk_sharecount++;
		COUNT(p, 1);
		/* fall into downgrade */

	case LK_DOWNGRADE:
		if (WEHOLDIT(lkp, pid, cpu_id) == 0 ||
		    lkp->lk_exclusivecount == 0)
			panic("lockmgr: not holding exclusive lock");
		lkp->lk_sharecount += lkp->lk_exclusivecount;
		lkp->lk_exclusivecount = 0;
		lkp->lk_flags &= ~LK_HAVE_EXCL;
		SETHOLDER(lkp, LK_NOPROC, LK_NOCPU);
		WAKEUP_WAITER(lkp);
		break;

	case LK_EXCLUPGRADE:
		/*
		 * If another process is ahead of us to get an upgrade,
		 * then we want to fail rather than have an intervening
		 * exclusive access.
		 */
		if (lkp->lk_flags & LK_WANT_UPGRADE) {
			lkp->lk_sharecount--;
			COUNT(p, -1);
			error = EBUSY;
			break;
		}
		/* fall into normal upgrade */

	case LK_UPGRADE:
		/*
		 * Upgrade a shared lock to an exclusive one. If another
		 * shared lock has already requested an upgrade to an
		 * exclusive lock, our shared lock is released and an
		 * exclusive lock is requested (which will be granted
		 * after the upgrade). If we return an error, the file
		 * will always be unlocked.
		 */
		if (WEHOLDIT(lkp, pid, cpu_id) || lkp->lk_sharecount <= 0)
			panic("lockmgr: upgrade exclusive lock");
		lkp->lk_sharecount--;
		COUNT(p, -1);
		/*
		 * If we are just polling, check to see if we will block.
		 */
		if ((extflags & LK_NOWAIT) &&
		    ((lkp->lk_flags & LK_WANT_UPGRADE) ||
		     lkp->lk_sharecount > 1)) {
			error = EBUSY;
			break;
		}
		if ((lkp->lk_flags & LK_WANT_UPGRADE) == 0) {
			/*
			 * We are first shared lock to request an upgrade, so
			 * request upgrade and wait for the shared count to
			 * drop to zero, then take exclusive lock.
			 */
			lkp->lk_flags |= LK_WANT_UPGRADE;
			ACQUIRE(lkp, error, extflags, lkp->lk_sharecount);
			lkp->lk_flags &= ~LK_WANT_UPGRADE;
			if (error)
				break;
			lkp->lk_flags |= LK_HAVE_EXCL;
			SETHOLDER(lkp, pid, cpu_id);
			if (lkp->lk_exclusivecount != 0)
				panic("lockmgr: non-zero exclusive count");
			lkp->lk_exclusivecount = 1;
			COUNT(p, 1);
			break;
		}
		/*
		 * Someone else has requested upgrade. Release our shared
		 * lock, awaken upgrade requestor if we are the last shared
		 * lock, then request an exclusive lock.
		 */
		if (lkp->lk_sharecount == 0)
			WAKEUP_WAITER(lkp);
		/* fall into exclusive request */

	case LK_EXCLUSIVE:
		if (WEHOLDIT(lkp, pid, cpu_id)) {
			/*
			 *	Recursive lock.
			 */
			if ((extflags & LK_CANRECURSE) == 0) {
				if (extflags & LK_RECURSEFAIL) {
					error = EDEADLK;
					break;
				}
				panic("lockmgr: locking against myself");
			}
			lkp->lk_exclusivecount++;
			COUNT(p, 1);
			break;
		}
		/*
		 * If we are just polling, check to see if we will sleep.
		 */
		if ((extflags & LK_NOWAIT) && ((lkp->lk_flags &
		     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) ||
		     lkp->lk_sharecount != 0)) {
			error = EBUSY;
			break;
		}
		/*
		 * Try to acquire the want_exclusive flag.
		 */
		ACQUIRE(lkp, error, extflags, lkp->lk_flags &
		    (LK_HAVE_EXCL | LK_WANT_EXCL));
		if (error)
			break;
		lkp->lk_flags |= LK_WANT_EXCL;
		/*
		 * Wait for shared locks and upgrades to finish.
		 */
		ACQUIRE(lkp, error, extflags, lkp->lk_sharecount != 0 ||
		       (lkp->lk_flags & LK_WANT_UPGRADE));
		lkp->lk_flags &= ~LK_WANT_EXCL;
		if (error)
			break;
		lkp->lk_flags |= LK_HAVE_EXCL;
		SETHOLDER(lkp, pid, cpu_id);
		if (lkp->lk_exclusivecount != 0)
			panic("lockmgr: non-zero exclusive count");
		lkp->lk_exclusivecount = 1;
		COUNT(p, 1);
		break;

	case LK_RELEASE:
		if (lkp->lk_exclusivecount != 0) {
			if (WEHOLDIT(lkp, pid, cpu_id) == 0) {
				if (lkp->lk_flags & LK_SPIN) {
					panic("lockmgr: processor %lu, not "
					    "exclusive lock holder %lu "
					    "unlocking", cpu_id, lkp->lk_cpu);
				} else {
					panic("lockmgr: pid %d, not "
					    "exclusive lock holder %d "
					    "unlocking", pid,
					    lkp->lk_lockholder);
				}
			}
			lkp->lk_exclusivecount--;
			COUNT(p, -1);
			if (lkp->lk_exclusivecount == 0) {
				lkp->lk_flags &= ~LK_HAVE_EXCL;
				SETHOLDER(lkp, LK_NOPROC, LK_NOCPU);
			}
		} else if (lkp->lk_sharecount != 0) {
			lkp->lk_sharecount--;
			COUNT(p, -1);
		} else
			panic("lockmgr: LK_RELEASE of unlocked lock");
		WAKEUP_WAITER(lkp);
		break;

	case LK_DRAIN:
		/*
		 * Check that we do not already hold the lock, as it can 
		 * never drain if we do. Unfortunately, we have no way to
		 * check for holding a shared lock, but at least we can
		 * check for an exclusive one.
		 */
		if (WEHOLDIT(lkp, pid, cpu_id))
			panic("lockmgr: draining against myself");
		/*
		 * If we are just polling, check to see if we will sleep.
		 */
		if ((extflags & LK_NOWAIT) && ((lkp->lk_flags &
		     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) ||
		     lkp->lk_sharecount != 0 || lkp->lk_waitcount != 0)) {
			error = EBUSY;
			break;
		}
		PAUSE(lkp, ((lkp->lk_flags &
		     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) ||
		     lkp->lk_sharecount != 0 || lkp->lk_waitcount != 0));
		for (error = 0; ((lkp->lk_flags &
		     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) ||
		     lkp->lk_sharecount != 0 || lkp->lk_waitcount != 0); ) {
			lkp->lk_flags |= LK_WAITDRAIN;
			simple_unlock(&lkp->lk_interlock);
			if ((error = tsleep((void *)&lkp->lk_flags, lkp->lk_prio,
			    lkp->lk_wmesg, lkp->lk_timo)) != 0)
				return (error);
			if ((extflags) & LK_SLEEPFAIL)
				return (ENOLCK);
			simple_lock(&lkp->lk_interlock);
		}
		lkp->lk_flags |= LK_DRAINING | LK_HAVE_EXCL;
		SETHOLDER(lkp, pid, cpu_id);
		lkp->lk_exclusivecount = 1;
		COUNT(p, 1);
		break;

	default:
		simple_unlock(&lkp->lk_interlock);
		panic("lockmgr: unknown locktype request %d",
		    flags & LK_TYPE_MASK);
		/* NOTREACHED */
	}
	if ((lkp->lk_flags & LK_WAITDRAIN) && ((lkp->lk_flags &
	     (LK_HAVE_EXCL | LK_WANT_EXCL | LK_WANT_UPGRADE)) == 0 &&
	     lkp->lk_sharecount == 0 && lkp->lk_waitcount == 0)) {
		lkp->lk_flags &= ~LK_WAITDRAIN;
		wakeup((void *)&lkp->lk_flags);
	}
	simple_unlock(&lkp->lk_interlock);
	return (error);
}

#ifdef notyet
/*
 * For a recursive spinlock held one or more times by the current CPU,
 * release all N locks, and return N.
 * Intended for use in mi_switch() shortly before context switching.
 */

int
#if defined(LOCKDEBUG)
_spinlock_release_all(__volatile struct lock *lkp, const char *file, int line)
#else
spinlock_release_all(__volatile struct lock *lkp)
#endif
{
	int s, count;
	cpuid_t cpu_id;
	
	KASSERT(lkp->lk_flags & LK_SPIN);
	
	INTERLOCK_ACQUIRE(lkp, LK_SPIN, s);

	cpu_id = cpu_number();
	count = lkp->lk_exclusivecount;
	
	if (count != 0) {
#ifdef DIAGNOSTIC		
		if (WEHOLDIT(lkp, 0, 0, cpu_id) == 0) {
			panic("spinlock_release_all: processor %lu, not "
			    "exclusive lock holder %lu "
			    "unlocking", (long)cpu_id, lkp->lk_cpu);
		}
#endif
		lkp->lk_recurselevel = 0;
		lkp->lk_exclusivecount = 0;
		COUNT_CPU(cpu_id, -count);
		lkp->lk_flags &= ~LK_HAVE_EXCL;
		SETHOLDER(lkp, LK_NOPROC, 0, LK_NOCPU);
#if defined(LOCKDEBUG)
		lkp->lk_unlock_file = file;
		lkp->lk_unlock_line = line;
#endif
		DONTHAVEIT(lkp);
	}
#ifdef DIAGNOSTIC
	else if (lkp->lk_sharecount != 0)
		panic("spinlock_release_all: release of shared lock!");
	else
		panic("spinlock_release_all: release of unlocked lock!");
#endif
	INTERLOCK_RELEASE(lkp, LK_SPIN, s);	

	return (count);
}

/*
 * For a recursive spinlock held one or more times by the current CPU,
 * release all N locks, and return N.
 * Intended for use in mi_switch() right after resuming execution.
 */

void
#if defined(LOCKDEBUG)
_spinlock_acquire_count(__volatile struct lock *lkp, int count,
    const char *file, int line)
#else
spinlock_acquire_count(__volatile struct lock *lkp, int count)
#endif
{
	int s, error;
	cpuid_t cpu_id;
	
	KASSERT(lkp->lk_flags & LK_SPIN);
	
	INTERLOCK_ACQUIRE(lkp, LK_SPIN, s);

	cpu_id = cpu_number();

#ifdef DIAGNOSTIC
	if (WEHOLDIT(lkp, LK_NOPROC, 0, cpu_id))
		panic("spinlock_acquire_count: processor %lu already holds lock", (long)cpu_id);
#endif
	/*
	 * Try to acquire the want_exclusive flag.
	 */
	ACQUIRE(lkp, error, LK_SPIN, 0, lkp->lk_flags &
	    (LK_HAVE_EXCL | LK_WANT_EXCL));
	lkp->lk_flags |= LK_WANT_EXCL;
	/*
	 * Wait for shared locks and upgrades to finish.
	 */
	ACQUIRE(lkp, error, LK_SPIN, 0, lkp->lk_sharecount != 0 ||
	    (lkp->lk_flags & LK_WANT_UPGRADE));
	lkp->lk_flags &= ~LK_WANT_EXCL;
	lkp->lk_flags |= LK_HAVE_EXCL;
	SETHOLDER(lkp, LK_NOPROC, 0, cpu_id);
#if defined(LOCKDEBUG)
	lkp->lk_lock_file = file;
	lkp->lk_lock_line = line;
#endif
	HAVEIT(lkp);
	if (lkp->lk_exclusivecount != 0)
		panic("lockmgr: non-zero exclusive count");
	lkp->lk_exclusivecount = count;
	lkp->lk_recurselevel = 1;
	COUNT_CPU(cpu_id, count);

	INTERLOCK_RELEASE(lkp, lkp->lk_flags, s);	
}
#endif

/*
 * Print out information about state of a lock. Used by VOP_PRINT
 * routines to display ststus about contained locks.
 */
void
lockmgr_printinfo(lkp)
	struct lock *lkp;
{

	if (lkp->lk_sharecount)
		printf(" lock type %s: SHARED (count %d)", lkp->lk_wmesg,
		    lkp->lk_sharecount);
	else if (lkp->lk_flags & LK_HAVE_EXCL)
		printf(" lock type %s: EXCL (count %d) by pid %d",
		    lkp->lk_wmesg, lkp->lk_exclusivecount, lkp->lk_lockholder);
	if (lkp->lk_waitcount > 0)
		printf(" with %d pending", lkp->lk_waitcount);
}

#if defined(LOCKDEBUG)

int lockdebug_print = 0;
int lockdebug_debugger = 0;

/*
 * Simple lock functions so that the debugger can see from whence
 * they are being called.
 */
void
simple_lock_init(lkp)
	struct simplelock *lkp;
{

	lkp->lock_data = SLOCK_UNLOCKED;
}

void
_simple_lock(lkp, id, l)
	__volatile struct simplelock *lkp;
	const char *id;
	int l;
{

	if (lkp->lock_data == SLOCK_LOCKED) {
		if (lockdebug_print)
			printf("%s:%d simple_lock: lock held...\n", id, l);
		if (lockdebug_debugger)
			Debugger();
	}
	lkp->lock_data = SLOCK_LOCKED;
}


int
_simple_lock_try(lkp, id, l)
	__volatile struct simplelock *lkp;
	const char *id;
	int l;
{

	if (lkp->lock_data == SLOCK_LOCKED) {
		if (lockdebug_print)
			printf("%s:%d simple_lock: lock held...\n", id, l);
		if (lockdebug_debugger)
			Debugger();
	}
	return lkp->lock_data = SLOCK_LOCKED;
}

void
_simple_unlock(lkp, id, l)
	__volatile struct simplelock *lkp;
	const char *id;
	int l;
{

	if (lkp->lock_data == SLOCK_UNLOCKED) {
		if (lockdebug_print)
			printf("%s:%d simple_unlock: lock not held...\n",
			       id, l);
		if (lockdebug_debugger)
			Debugger();
	}
	lkp->lock_data = SLOCK_UNLOCKED;
}

void
_simple_lock_assert(lkp, state, id, l)
	__volatile struct simplelock *lkp;
	int state;
	const char *id;
	int l;
{
	if (lkp->lock_data != state) {
		if (lockdebug_print)
			printf("%s:%d simple_lock_assert: wrong state: %d",
			       id, l, lkp->lock_data);
		if (lockdebug_debugger)
			Debugger();
	}
}
#endif /* LOCKDEBUG */

#if defined(MULTIPROCESSOR)
/*
 * Functions for manipulating the kernel_lock.  We put them here
 * so that they show up in profiles.
 */

struct lock kernel_lock; 

void
_kernel_lock_init(void)
{
	spinlockinit(&kernel_lock, "klock", 0);
}

/*
 * Acquire/release the kernel lock.  Intended for use in the scheduler
 * and the lower half of the kernel.
 */
void
_kernel_lock(int flag)
{
	SCHED_ASSERT_UNLOCKED();
	spinlockmgr(&kernel_lock, flag, 0);
}

void
_kernel_unlock(void)
{
	spinlockmgr(&kernel_lock, LK_RELEASE, 0);
}

/*
 * Acquire/release the kernel_lock on behalf of a process.  Intended for
 * use in the top half of the kernel.
 */
void
_kernel_proc_lock(struct proc *p)
{
	SCHED_ASSERT_UNLOCKED();
	spinlockmgr(&kernel_lock, LK_EXCLUSIVE, 0);
	p->p_flag |= P_BIGLOCK;
}

void
_kernel_proc_unlock(struct proc *p)
{
	p->p_flag &= ~P_BIGLOCK;
	spinlockmgr(&kernel_lock, LK_RELEASE, 0);
}
#endif /* MULTIPROCESSOR */
