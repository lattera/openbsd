/*	$OpenBSD: src/sys/arch/mvme88k/dev/memdevs.h,v 1.1.8.1 2002/06/11 03:37:09 art Exp $	*/

#ifndef _MVME88K_MEMDEVS_H_
#define _MVME88K_MEMDEVS_H_

int memdevrw(caddr_t base, int len, struct uio *uio, int flags);

#endif	/* _MVME88K_MEMDEVS_H_ */
