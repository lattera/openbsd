/*	$OpenBSD: src/sys/arch/mvme88k/dev/memdevs.h,v 1.1.2.1 2001/04/18 16:10:58 niklas Exp $	*/

#ifndef _MVME88K_MEMDEVS_H_
#define _MVME88K_MEMDEVS_H_

int memdevrw __P((caddr_t base, int len, struct uio *uio, int flags));

#endif	/* _MVME88K_MEMDEVS_H_ */
