#	$OpenBSD: src/regress/libexec/ld.so/initfirst/test1/libif2/Makefile,v 1.1 2011/11/29 04:36:15 kurt Exp $

LIB=		if2
SRCS=		lib.c
LDADD+=		-Wl,-zinitfirst

regress: all

.include <bsd.lib.mk>
