#	$OpenBSD: src/sys/lib/libkern/arch/arm32/Attic/Makefile.inc,v 1.3.10.1 2001/05/14 22:32:48 niklas Exp $

SRCS+=	__main.c imax.c imin.c lmax.c lmin.c max.c min.c ulmax.c ulmin.c \
	memchr.c memcmp.c \
	bcmp.c bzero.c ffs.c strcat.c strcmp.c strcpy.c strlcat.c strlcpy.c \
	strlen.c strncmp.c \
	strncpy.c scanc.c skpc.c locc.c htonl.c htons.c ntohl.c ntohs.c \
	div.S random.c strncasecmp.c
