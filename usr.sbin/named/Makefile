#	$OpenBSD: src/usr.sbin/named/Attic/Makefile,v 1.11.4.1 2002/11/14 23:44:14 millert Exp $

SUBDIR= named named-xfer ndc reload restart dig nslookup \
	host dnsquery
.if make(install)
SUBDIR+= doc/bog
.endif

VER= 4.9.11-REL

.include <bsd.subdir.mk>
