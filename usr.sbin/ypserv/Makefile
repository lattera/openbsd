#	from: @(#)Makefile	5.8 (Berkeley) 7/28/90
#	$Id: Makefile,v 1.6 1995/01/29 21:28:38 moj Exp $

PROG=	ypserv
SRCS=	ypserv.c ypserv_xdr.c ypserv_proc.c ypserv_db.c acl.c yplog.c ypdb.c
MAN=	ypserv.acl.5 securenet.5 ypserv.8
CFLAGS+=-DDAEMON -DOPTDB

.include <bsd.prog.mk>
