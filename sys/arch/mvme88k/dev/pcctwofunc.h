/*	$OpenBSD: src/sys/arch/mvme88k/dev/Attic/pcctwofunc.h,v 1.1.2.2 2002/03/28 10:36:02 niklas Exp $ */

#ifndef _MVME88K_PCCTWO_H_
#define _MVME88K_PCCTWO_H_

int pcctwointr_establish(int vec, struct intrhand *ih);

#endif	/* _MVME88K_PCCTWO_H_ */

