#	$OpenBSD: src/sys/lib/libkern/arch/m88k/Attic/Makefile.inc,v 1.2.6.1 2001/05/14 22:32:51 niklas Exp $
#	$NetBSD: Makefile.inc,v 1.2 1996/04/13 01:17:58 cgd Exp $

SRCS+=	__main.c imax.c imin.c lmax.c lmin.c max.c min.c ulmax.c ulmin.c \
	memchr.c memcmp.c memset.c bcmp.c random.c \
	strcat.c strcmp.c strcpy.c strlcat.c strlcpy.c strlen.c strncmp.c \
	strncpy.c scanc.c skpc.c strncasecmp.c ffs.c
