#	$OpenBSD: src/sys/arch/mvme88k/stand/libbug/Makefile,v 1.5.4.1 2004/02/19 10:49:09 niklas Exp $

LIB=bug

NOPIC=
NOPROFILE=

S=${.CURDIR}/../../../..
DIR_SA=$S/lib/libsa

CFLAGS+=-I${.CURDIR}/../../include -I${DIR_SA}

SRCS=	delay.c diskrd.c diskwr.c getbrdid.c inchr.c instat.c \
	netfopen.c netfread.c outln.c outstr.c putchar.c return.c rtc_rd.c

install:

.include <bsd.lib.mk>
