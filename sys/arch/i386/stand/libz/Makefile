#	$OpenBSD: src/sys/arch/i386/stand/libz/Attic/Makefile,v 1.10.6.1 2004/02/19 10:48:43 niklas Exp $
#	$NetBSD: Makefile,v 1.2 1997/01/22 01:36:30 cgd Exp $

S=${.CURDIR}/../../../..
ZDST=${.OBJDIR}
SADIR=${.CURDIR}/..

.PATH: ${S}/lib/libz

.include "${S}/lib/libz/Makefile"
CFLAGS+=${SACFLAGS} -I${S}/lib/libsa ${DEBUGFLAGS}
