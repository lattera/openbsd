#	$OpenBSD: src/usr.sbin/named/Attic/Makefile,v 1.10.4.1 2001/01/30 00:06:40 jason Exp $

SUBDIR= named named-xfer ndc reload restart dig nslookup \
	host dnsquery
.if make(install)
SUBDIR+= doc/bog
.endif

VER= 4.9.8-REL

.include <bsd.subdir.mk>
