#	$OpenBSD: src/usr.sbin/npppd/Makefile,v 1.6 2013/03/14 16:20:46 giovanni Exp $
#
# $Id: Makefile,v 1.5 2012/09/20 12:51:43 yasuoka Exp $
#

SUBDIR+=	npppd

distribution:
	${INSTALL} -C -o root -g wheel -m 0640 ${.CURDIR}/npppd/npppd.conf \
	    ${DESTDIR}/etc/npppd/npppd.conf
	${INSTALL} -C -o root -g wheel -m 0600 ${.CURDIR}/npppd/npppd-users \
	    ${DESTDIR}/etc/npppd/npppd-users

.include <bsd.subdir.mk>
