#	$OpenBSD: src/sys/arch/hppa/stand/libz/Makefile,v 1.1.1.1.8.1 2004/02/19 10:48:41 niklas Exp $

S=${.CURDIR}/../../../..
ZDST=${.OBJDIR}
SADIR=${.CURDIR}/..

.PATH: ${S}/lib/libz

.include "${S}/lib/libz/Makefile"
CFLAGS+=${SACFLAGS} -I${S}/lib/libsa
