/*	$OpenBSD: src/sys/arch/mvmeppc/include/Attic/conf.h,v 1.1.2.1 2002/06/11 03:37:22 art Exp $	*/

#include <sys/conf.h>

#define	mmread	mmrw
#define	mmwrite	mmrw
cdev_decl(mm);
