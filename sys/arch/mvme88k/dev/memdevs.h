/*	$OpenBSD: src/sys/arch/mvme88k/dev/memdevs.h,v 1.1.2.2 2002/03/28 10:36:02 niklas Exp $	*/

#ifndef _MVME88K_MEMDEVS_H_
#define _MVME88K_MEMDEVS_H_

int memdevrw(caddr_t base, int len, struct uio *uio, int flags);

#endif	/* _MVME88K_MEMDEVS_H_ */
