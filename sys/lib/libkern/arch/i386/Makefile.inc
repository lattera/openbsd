#	$OpenBSD: src/sys/lib/libkern/arch/i386/Attic/Makefile.inc,v 1.6.2.1 2000/12/19 22:58:39 jason Exp $
#	$NetBSD: Makefile.inc,v 1.10 1996/04/13 01:17:41 cgd Exp $

SRCS+=	__main.c imax.c imin.c lmax.c lmin.c max.c min.c ulmax.c ulmin.c \
	memchr.S memcmp.S \
	bcmp.S ffs.S memset.S strcat.S strcmp.S strcpy.S strlcat.c strlcpy.c \
	strlen.S strncmp.c \
	strncpy.c scanc.S skpc.S locc.S htonl.S htons.S ntohl.S ntohs.S \
	strncasecmp.c
