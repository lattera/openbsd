/*	$OpenBSD: src/lib/libc/arch/hppa/gen/Attic/sigsetjmp.S,v 1.3 2001/06/04 23:14:01 mickey Exp $	*/

#include "SYS.h"

#ifdef SYSLIBC_SCCS
	.text
	.asciz "$OpenBSD: src/lib/libc/arch/hppa/gen/Attic/sigsetjmp.S,v 1.3 2001/06/04 23:14:01 mickey Exp $"
	.align	4
#endif

ENTRY(sigsetjmp,0)
	bv	r0(rp)
	nop
EXIT(sigsetjmp)

ENTRY(siglongjmp,0)
	bv	r0(rp)
	nop
EXIT(siglongjmp)

	.end
