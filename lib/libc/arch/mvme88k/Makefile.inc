#	$Id: Makefile.inc,v 1.2 1995/11/20 14:07:50 build Exp build $

KMINCLUDES=	arch/m88k/DEFS.h arch/m88k/SYS.h
#KMSRCS=	bcmp.S bzero.S ffs.S strcat.c strcmp.S strcpy.S strlen.S strncmp.S \
#	strncpy.S htonl.S htons.S ntohl.S ntohs.S
KMSRCS=	htonl.S htons.S ntohl.S ntohs.S
