#	$Id$

PROG=	mk_cmds
SRCS=	mk_cmds.c options.c utils.c ct.c cmd_tbl.c
CFLAGS+=-I. -I${.CURDIR}/../ss
LFLAGS=	-l
LDADD+=	-ll
DPADD+=	${LIBL}
CLEANFILES+=	y.tab.c y.tab.h lex.yy.c cmd_tbl.c ct.c
BINDIR=	/usr/bin
NOMAN=	noman

# don't install -- only needed during build of kerberos (so far)
install:

.include <bsd.prog.mk>
