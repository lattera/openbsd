#	$OpenBSD: src/sys/dev/cardbus/Attic/Makefile,v 1.1.4.1 2001/05/14 22:23:08 niklas Exp $
#	$NetBSD: Makefile.cardbusdevs,v 1.2 1999/10/15 06:42:21 haya Exp $

AWK=	awk

cardbusdevs.h cardbusdevs_data.h: cardbusdevs devlist2h.awk
	/bin/rm -f cardbusdevs.h cardbusdevs_data.h
	${AWK} -f devlist2h.awk cardbusdevs
