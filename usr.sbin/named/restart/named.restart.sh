#!/bin/sh -
#
#	$OpenBSD: src/usr.sbin/named/restart/Attic/named.restart.sh,v 1.2 1997/03/12 10:42:53 downsj Exp $
#	from named.restart	5.4 (Berkeley) 6/27/89
#	$Id: named.restart.sh,v 8.1 1994/12/15 06:24:14 vixie Exp $
#

exec %DESTSBIN%/%INDOT%ndc restart
