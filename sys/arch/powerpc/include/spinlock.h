/*	$OpenBSD: src/sys/arch/powerpc/include/spinlock.h,v 1.1.6.1 2001/10/31 03:07:55 nate Exp $	*/

#ifndef _POWERPC_SPINLOCK_H_
#define _POWERPC_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0)
#define _SPINLOCK_LOCKED	(1)
typedef int _spinlock_lock_t;

#endif
