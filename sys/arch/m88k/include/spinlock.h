/*	$OpenBSD: src/sys/arch/m88k/include/spinlock.h,v 1.1.2.1 2004/06/05 23:09:48 niklas Exp $	*/

#ifndef _M88K_SPINLOCK_H_
#define _M88K_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0)
#define _SPINLOCK_LOCKED	(1)
typedef int _spinlock_lock_t;

#endif
