#	$OpenBSD: src/regress/libexec/ld.so/dlclose/test2/libcc/Makefile,v 1.1.1.1 2005/09/30 14:51:52 kurt Exp $

LIB=		cc
SRCS=		cc.c
LDADD+=		-Wl,-E
LDADD+=		-Wl,-rpath,$(ZZ_OBJDIR)
LDADD+=		-L$(ZZ_OBJDIR) -lzz

regress: all

.include <bsd.lib.mk>
