/*	$OpenBSD: src/sys/arch/mvme88k/dev/Attic/sysconfunc.h,v 1.1.8.1 2002/06/11 03:37:10 art Exp $	*/

#ifndef _MVME88K_SYSCON_H_
#define _MVME88K_SYSCON_H_

int sysconintr_establish(int vec, struct intrhand *ih);

#endif	/* _MVME88K_SYSCON_H_ */
