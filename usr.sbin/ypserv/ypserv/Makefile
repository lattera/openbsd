#	from: @(#)Makefile	5.8 (Berkeley) 7/28/90
#	$Id: Makefile,v 1.8 1995/10/24 21:35:29 moj Exp $

PROG=	ypserv
SRCS=	ypserv.c ypserv_proc.c ypserv_db.c acl.c yplog.c ypdb.c ypserv_xdr.c
MAN=	ypserv.acl.5 securenet.5 ypserv.8
.PATH:	${.CURDIR}/../common
CFLAGS+=-DDAEMON -I${.CURDIR}/../common

.include <bsd.prog.mk>
