#	$OpenBSD: src/sys/arch/sparc64/stand/libkern/Attic/Makefile,v 1.3.6.1 2003/05/19 21:46:58 tedu Exp $

LIB=	kern

.PATH:	${.CURDIR}/../../../../lib/libkern
.PATH:	${.CURDIR}/../../../../lib/libkern/arch/sparc

.include "${.CURDIR}/../../../../lib/libkern/arch/sparc/Makefile.inc"

CFLAGS= -O2 -nostdinc -I${.CURDIR}/../include -I${.CURDIR}/../../../.. \
	-I${.CURDIR}/../../../../lib/libkern \
	-I${.CURDIR}/../../../../lib/libkern/arch/sparc -I${.CURDIR}

AFLAGS=	-x assembler-with-cpp -traditional-cpp -I${.CURDIR}/../include \
	-I${.CURDIR}/../../../..  -I${.CURDIR}/../../../../lib/libkern \
	-I${.CURDIR}/../../../../lib/libkern/arch/sparc

# kern routines
SRCS=	__main.c ashrdi3.c bzero.S divdi3.c qdivrem.c strlen.S strcat.c \
	strcmp.c strcpy.c strlcat.c strlcpy.c strncmp.c umoddi3.c

SRCS+=	umul.S mul.S rem.S sdiv.S udiv.S urem.S
SRCS+=	saveregs.S

${OBJS}:	${.CURDIR}/../Makefile.inc

CLEANFILES += machine

NOPROFILE=
NOPIC=

.if !make(libdep) && !make(sadep) && !make(salibdir) && !make(kernlibdir) && !make(obj)
.BEGIN:
	@([ -h machine ] || ln -s ${.CURDIR}/../../include machine)
.endif

install:

.include <bsd.lib.mk>
