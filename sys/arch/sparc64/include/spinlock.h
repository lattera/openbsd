/*	$OpenBSD: src/sys/arch/sparc64/include/spinlock.h,v 1.1.4.1 2001/10/31 03:07:58 nate Exp $	*/

#ifndef _MACHINE_SPINLOCK_H_
#define _MACHINE_SPINLOCK_H_

#define _SPINLOCK_UNLOCKED	(0x00)
#define _SPINLOCK_LOCKED	(0xFF)
typedef unsigned char _spinlock_lock_t;

#endif
