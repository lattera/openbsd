#	$OpenBSD: src/sbin/routed/Attic/Makefile,v 1.1.1.2 1996/09/05 13:58:44 mickey Exp $
#	@(#)Makefile	8.1 (Berkeley) 6/19/93

PROG=	routed
SRCS=	if.c input.c main.c output.c parms.c radix.c rdisc.c table.c trace.c
MAN=	routed.8
CFLAGS+=-D_ROUTED -D'min(a,b)=(a < b ? a : b)' -D'log=syslog'
CFLAGS+=-D'panic(s)={log(LOG_ERR,s); exit(1);}'
SUBDIR=	rtquery
DPADD=	${LIBCOMPAT}
LDADD=	-lcompat

.PATH: ${.CURDIR}/../../sys/net

.include <bsd.prog.mk>
