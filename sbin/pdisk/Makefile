#	$Id: Makefile,v 1.5 2002/04/20 22:51:32 miod Exp $

.if ${MACHINE} == "macppc" || ${MACHINE} == "mac68k"
PROG=	pdisk

SRCS=	bitfield.c convert.c deblock_media.c dump.c errors.c \
	file_media.c io.c media.c partition_map.c pathname.c \
	pdisk.c util.c validate.c

.else
NOPROG=yes
.endif

MAN=	pdisk.8
MANSUBDIR=macppc mac68k

.include <bsd.prog.mk>

