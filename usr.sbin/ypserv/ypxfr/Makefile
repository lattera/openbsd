#	from: @(#)Makefile	5.8 (Berkeley) 7/28/90
#	$Id: Makefile,v 1.4 1995/09/09 22:58:32 moj Exp $

PROG=	ypxfr
SRCS=	ypxfr.c ypdb.c yplog.c yplib_host.c ypxfr_xdr.c
MAN=	ypxfr.8
.PATH:	${.CURDIR}/../common
CFLAGS+=-I${.CURDIR}/../common

.include <bsd.prog.mk>
