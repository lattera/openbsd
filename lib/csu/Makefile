#	$Id: Makefile,v 1.1.1.1 1995/10/18 08:41:17 deraadt Exp $

.if make(obj)
SUBDIR= alpha i386 m68k mips mvme88k ns32k powerpc sparc vax
.else
SUBDIR= ${MACHINE_ARCH}
.endif

SUBDIR+= c++

.include <bsd.subdir.mk>
