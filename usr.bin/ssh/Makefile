#	$OpenBSD: src/usr.bin/ssh/Makefile,v 1.8.2.1 2001/09/27 19:03:54 jason Exp $

.include <bsd.own.mk>

SUBDIR=	lib ssh sshd ssh-add ssh-keygen ssh-agent scp sftp-server \
	ssh-keyscan sftp scard

distribution:
	install -C -o root -g wheel -m 0644 ${.CURDIR}/ssh_config \
	    ${DESTDIR}/etc/ssh_config
	install -C -o root -g wheel -m 0644 ${.CURDIR}/sshd_config \
	    ${DESTDIR}/etc/sshd_config

.include <bsd.subdir.mk>
