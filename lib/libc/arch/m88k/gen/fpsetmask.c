/*	$OpenBSD: src/lib/libc/arch/m88k/gen/fpsetmask.c,v 1.2 2003/01/07 22:01:29 miod Exp $	*/

/*
 * Written by J.T. Conklin, Apr 10, 1995
 * Public domain.
 * Porting to m88k by Nivas Madhur.
 */

#include <ieeefp.h>

#if defined(LIBC_SCCS) && !defined(lint)
static char rcsid[] = "$OpenBSD: src/lib/libc/arch/m88k/gen/fpsetmask.c,v 1.2 2003/01/07 22:01:29 miod Exp $";
#endif /* LIBC_SCCS and not lint */

fp_except
fpsetmask(mask)
	fp_except mask;
{
	fp_except old;
	fp_except new;

	__asm__ volatile("fldcr %0,fcr63" : "=r" (old));

	new = old;
	new &= ~0x1f;		/* clear bottom 5 bits and */
	new |= (mask & 0x1f);	/* set them to mask */

	__asm__ volatile("fstcr %0,fcr63" : : "r" (new));

	return (old & 0x1f);
}
