#
# $Id: Makefile,v 1.1 2010/01/13 07:49:44 yasuoka Exp $
#
.include <bsd.own.mk>

PROG=		npppdctl
SRCS=		npppdctl.c debugutil.c
MAN=		npppdctl.8

CPPFLAGS+=	-I${.CURDIR}/../npppd -I${.CURDIR}/../common

.include <bsd.prog.mk>
.PATH:		${.CURDIR}/../common
