#
# $Id: Makefile 36168 2009-08-20 09:59:47Z yasuoka $
#
NOMAN=		#defined

.include <bsd.own.mk>

PROG=		npppdctl
SRCS=		npppdctl.c debugutil.c

CPPFLAGS+=	-I${.CURDIR}/../npppd -I${.CURDIR}/../common

.include <bsd.prog.mk>
.PATH:		${.CURDIR}/../common
