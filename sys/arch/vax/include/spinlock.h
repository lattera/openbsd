/*	$OpenBSD: src/sys/arch/vax/include/spinlock.h,v 1.1.8.1 2001/05/14 21:38:06 niklas Exp $	*/

#ifndef _MACHINE_SPINLOCK_H_
#define _MACHINE_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(1)
#define _SPINLOCK_LOCKED	(0)
typedef int _spinlock_lock_t;

#endif
