#	$OpenBSD: src/regress/libexec/ld.so/dlsym/test3/prog4/Makefile,v 1.1 2005/09/19 21:50:27 drahn Exp $

PROG=		prog4
SRCS=		main.c
LDFLAGS+=	-Wl,-E
LDFLAGS+=	-Wl,-rpath,$(BB_OBJDIR)
LDFLAGS+=	-Wl,-rpath,$(CC_OBJDIR)

.include <bsd.regress.mk>
