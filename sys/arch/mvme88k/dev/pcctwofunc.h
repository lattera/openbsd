/*	$OpenBSD: src/sys/arch/mvme88k/dev/Attic/pcctwofunc.h,v 1.1.2.1 2001/04/18 16:11:00 niklas Exp $ */

#ifndef _MVME88K_PCCTWO_H_
#define _MVME88K_PCCTWO_H_

int pcctwointr_establish __P((int vec, struct intrhand *ih));

#endif	/* _MVME88K_PCCTWO_H_ */

