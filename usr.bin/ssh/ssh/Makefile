#	$OpenBSD: src/usr.bin/ssh/ssh/Makefile,v 1.42.2.1 2003/09/16 21:20:30 brad Exp $

.PATH:		${.CURDIR}/..

PROG=	ssh
BINOWN=	root

#BINMODE?=4555

BINDIR=	/usr/bin
MAN=	ssh.1 ssh_config.5
LINKS=	${BINDIR}/ssh ${BINDIR}/slogin
MLINKS=	ssh.1 slogin.1

SRCS=	ssh.c readconf.c clientloop.c sshtty.c \
	sshconnect.c sshconnect1.c sshconnect2.c

.include <bsd.own.mk> # for AFS

#.if (${KERBEROS5:L} == "yes")
#CFLAGS+= -DKRB5 -I${DESTDIR}/usr/include/kerberosV -DGSSAPI 
#.endif # KERBEROS5

.include <bsd.prog.mk>

#.if (${KERBEROS5:L} == "yes")
#DPADD+=  ${LIBGSSAPI} ${LIBKRB5}
#LDADD+=  -lgssapi -lkrb5
#.endif # KERBEROS5

DPADD+=	${LIBCRYPTO} ${LIBZ} ${LIBDES}
LDADD+=	-lcrypto -lz -ldes
