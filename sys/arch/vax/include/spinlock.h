/*	$OpenBSD: src/sys/arch/vax/include/spinlock.h,v 1.1.8.2 2003/03/27 23:52:19 niklas Exp $	*/

#ifndef _MACHINE_SPINLOCK_H_
#define _MACHINE_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0)
#define _SPINLOCK_LOCKED	(1)
typedef int _spinlock_lock_t;

#endif
