/*	$OpenBSD: src/sys/arch/mvme88k/dev/Attic/sysconfunc.h,v 1.1.2.1 2001/04/18 16:11:05 niklas Exp $	*/

#ifndef _MVME88K_SYSCON_H_
#define _MVME88K_SYSCON_H_

int sysconintr_establish __P((int vec, struct intrhand *ih));

#endif	/* _MVME88K_SYSCON_H_ */
