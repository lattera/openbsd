#	$OpenBSD: src/sys/compat/hpux/Attic/Makefile,v 1.1.16.1 2001/10/31 03:11:46 nate Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	hpux_sysent.c hpux_syscalls.c hpux_syscall.h hpux_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
