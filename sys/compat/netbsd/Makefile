#	$OpenBSD: src/sys/compat/netbsd/Attic/Makefile,v 1.1.4.1 2001/10/31 03:11:46 nate Exp $

SYS=	../..
DEP=	syscalls.conf syscalls.master ${SYS}/kern/makesyscalls.sh
OBJS=	netbsd_sysent.c netbsd_syscalls.c \
	netbsd_syscall.h netbsd_syscallargs.h

${OBJS}: ${DEP}
	sh ${SYS}/kern/makesyscalls.sh syscalls.conf syscalls.master
