#	$OpenBSD: src/sys/lib/libkern/arch/vax/Attic/Makefile.inc,v 1.5.2.1 2000/12/19 22:58:40 jason Exp $
#	$NetBSD: Makefile.inc,v 1.2 1996/04/13 01:17:58 cgd Exp $

SRCS+=	__main.c imax.c imin.c lmax.c lmin.c max.c min.c ulmax.c ulmin.c \
	memchr.c memcmp.c \
	strcat.c strcmp.c strcpy.c strlcat.c strlcpy.c strlen.c strncmp.c \
	strncpy.c scanc.c skpc.c strncasecmp.c
