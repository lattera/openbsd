/*
 * COPYRIGHT
 */
#if defined(SYSLIBC_SCCS)
	.text
	.asciz "$OpenBSD: src/lib/libc/arch/powerpc/sys/exect.S,v 1.5 2002/10/07 04:22:46 drahn Exp $"
#endif /* SYSLIBC_SCCS */

#include "SYS.h"

PSEUDO(exect,execve)
