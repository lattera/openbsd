/*
 * Written by Michael Shalayeff. Public Domain
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$OpenBSD: src/lib/libm/arch/hppa/s_rint.c,v 1.2 2002/09/11 15:16:52 mickey Exp $";
#endif

#include "math.h"

double
rint(double x)
{
	__asm__ __volatile__("frnd,dbl %0,%0" : "+f" (x));

	return (x);
}
