#	$OpenBSD: src/usr.bin/ssh/Makefile,v 1.11.4.1 2004/03/04 18:18:14 brad Exp $

.include <bsd.own.mk>

SUBDIR=	lib ssh sshd ssh-add ssh-keygen ssh-agent scp sftp-server \
	ssh-keysign ssh-keyscan sftp scard

distribution:
	${INSTALL} -C -o root -g wheel -m 0644 ${.CURDIR}/ssh_config \
	    ${DESTDIR}/etc/ssh/ssh_config
	${INSTALL} -C -o root -g wheel -m 0644 ${.CURDIR}/sshd_config \
	    ${DESTDIR}/etc/ssh/sshd_config

.include <bsd.subdir.mk>
