/*	$OpenBSD: src/sys/arch/arm/include/spinlock.h,v 1.1.2.1 2004/02/19 10:48:02 niklas Exp $	*/

#ifndef _ARM_SPINLOCK_H_
#define _ARM_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0)
#define _SPINLOCK_LOCKED	(1)
typedef int _spinlock_lock_t;

#endif
