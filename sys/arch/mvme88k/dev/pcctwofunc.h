/*	$OpenBSD: src/sys/arch/mvme88k/dev/Attic/pcctwofunc.h,v 1.3.2.1 2002/06/11 03:37:10 art Exp $ */

#ifndef _MVME88K_PCCTWO_H_
#define _MVME88K_PCCTWO_H_

int pcctwointr_establish(int vec, struct intrhand *ih);

#endif	/* _MVME88K_PCCTWO_H_ */

