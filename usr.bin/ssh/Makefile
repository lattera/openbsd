#	$OpenBSD: src/usr.bin/ssh/Makefile,v 1.10.2.2 2002/06/26 15:30:37 jason Exp $

.include <bsd.own.mk>

SUBDIR=	lib ssh sshd ssh-add ssh-keygen ssh-agent scp sftp-server \
	ssh-keysign ssh-keyscan sftp scard

distribution:
	install -C -o root -g wheel -m 0644 ${.CURDIR}/ssh_config \
	    ${DESTDIR}/etc/ssh/ssh_config
	install -C -o root -g wheel -m 0644 ${.CURDIR}/sshd_config \
	    ${DESTDIR}/etc/ssh/sshd_config

beforeinstall:
	install -d -m 0755 -o root -g wheel ${DESTDIR}/var/empty

.include <bsd.subdir.mk>
