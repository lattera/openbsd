#	$OpenBSD: src/sys/arch/vax/boot/Attic/Makefile.inc,v 1.2.6.2 2003/03/27 23:52:19 niklas Exp $
#	$NetBSD: Makefile.inc,v 1.6 2002/02/24 01:04:23 matt Exp $

RELOC=0x39F000
.PATH:	${.CURDIR}/../../vax ${.CURDIR}/../common

CFLAGS+=	-fno-stack-protector
CPPFLAGS+=-I. -I${.CURDIR}/../../../../ -I${.CURDIR}/../../ -I${.CURDIR}/../common -I${.CURDIR}/../../include
CPPFLAGS+=-DRELOC=${RELOC}

# Private handling of assembler files.
.s.o:
	${CC} -x assembler-with-cpp ${CPPFLAGS} -E ${.IMPSRC} | as -o ${.TARGET}
