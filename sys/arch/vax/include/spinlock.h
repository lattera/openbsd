/*	$OpenBSD: src/sys/arch/vax/include/spinlock.h,v 1.1.12.1 2003/05/19 21:46:09 tedu Exp $	*/

#ifndef _MACHINE_SPINLOCK_H_
#define _MACHINE_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0)
#define _SPINLOCK_LOCKED	(1)
typedef int _spinlock_lock_t;

#endif
