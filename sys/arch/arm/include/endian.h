/*	$OpenBSD: src/sys/arch/arm/include/endian.h,v 1.1.2.1 2004/02/19 10:48:02 niklas Exp $	*/

#ifdef __ARMEB__
#define BYTE_ORDER BIG_ENDIAN
#else
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#include <sys/endian.h>
