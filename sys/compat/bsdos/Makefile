#	$OpenBSD: src/sys/compat/bsdos/Attic/Makefile,v 1.1.12.1 2001/10/31 03:11:45 nate Exp $

SYS=	../..
DEP=	syscalls.conf syscalls.master ${SYS}/kern/makesyscalls.sh
OBJS=	bsdos_sysent.c bsdos_syscalls.c \
	bsdos_syscall.h bsdos_syscallargs.h

${OBJS}: ${DEP}
	sh ${SYS}/kern/makesyscalls.sh syscalls.conf syscalls.master
