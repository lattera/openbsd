#	$OpenBSD: src/sys/compat/svr4/Attic/Makefile,v 1.2.16.1 2001/10/31 03:11:47 nate Exp $
#	$NetBSD: Makefile,v 1.1 1995/10/14 20:24:14 christos Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	svr4_sysent.c svr4_syscalls.c svr4_syscall.h svr4_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
