#	$OpenBSD: src/sys/arch/vax/boot/Attic/Makefile.inc,v 1.2.6.1 2001/05/14 21:37:39 niklas Exp $
#	$NetBSD: Makefile.inc,v 1.2 1999/07/18 05:55:45 abs Exp $

XXRPB=0x354240
RELOC=0x39F000
.PATH:	${.CURDIR}/../../vax ${.CURDIR}/../common

CPPFLAGS+=-I${.CURDIR}/../../../../ -I${.CURDIR}/../../ -I${.CURDIR}/../common -I${.CURDIR}/../../include
CPPFLAGS+=-DXXRPB=${XXRPB} -DRELOC=${RELOC}

# Private handling of assembler files.
.s.o:
	${CC} -x assembler-with-cpp ${CPPFLAGS} -E ${.IMPSRC} | as -o ${.TARGET}
