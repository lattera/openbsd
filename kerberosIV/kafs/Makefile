#	$Id: Makefile,v 1.2 1997/11/28 12:48:42 art Exp $

LIB=	kafs
CFLAGS+=-I${.CURDIR}
SRCS=	afskrb.c afssys.c common.c

.include <bsd.own.mk> # for AFS

.if (${AFS} != "yes")
CFLAGS+=-DNO_AFS
.endif

.include <bsd.lib.mk>
