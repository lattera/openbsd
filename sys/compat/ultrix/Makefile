#	$OpenBSD: src/sys/compat/ultrix/Attic/Makefile,v 1.1.16.1 2001/10/31 03:11:47 nate Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	ultrix_sysent.c ultrix_syscalls.c ultrix_syscall.h ultrix_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
