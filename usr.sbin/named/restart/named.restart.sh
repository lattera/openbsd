#!/bin/sh -
#
#	$OpenBSD: src/usr.sbin/named/restart/Attic/named.restart.sh,v 1.3 1997/03/12 14:51:59 downsj Exp $
#	from named.restart	5.4 (Berkeley) 6/27/89
#	$Id: named.restart.sh,v 1.2 1997/03/12 10:42:53 downsj Exp $
#

exec %DESTSBIN%/ndc restart
