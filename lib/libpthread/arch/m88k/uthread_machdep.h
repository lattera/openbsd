/*
 * OpenBSD/m88k machine-dependent thread macros
 *
 * $OpenBSD: src/lib/libpthread/arch/m88k/Attic/uthread_machdep.h,v 1.1 1999/03/03 06:00:10 smurph Exp $
 */

/* save the floating point state of a thread */
#define _thread_machdep_save_float_state(thr) 		\
	{						\
		/* fsave privileged instr */		\
	}

/* restore the floating point state of a thread */
#define _thread_machdep_restore_float_state(thr) 	\
	{						\
		/* frestore privileged instr */		\
	}

/* initialise the jmpbuf stack frame so it continues from entry */

#define _thread_machdep_thread_create(thr, entry, pattr)	\
	{						\
	    /* entry */					\
	    (thr)->saved_jmp_buf[5] = (long) entry;	\
	    /* stack */					\
	    (thr)->saved_jmp_buf[2] = (long) (thr)->stack \
				+ (pattr)->stacksize_attr \
				- sizeof(double);	\
	}

#define	_thread_machdep_longjmp(a,v)	_longjmp(a,v)
#define	_thread_machdep_setjmp(a)	_setjmp(a)

typedef jmp_buf _machdep_jmp_buf;

struct _machdep_struct {
        /* char            saved_fp[108]; */
	int	dummy;
};

