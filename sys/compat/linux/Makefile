#	$OpenBSD: src/sys/compat/linux/Makefile,v 1.2.16.1 2001/10/31 03:11:46 nate Exp $
#	$NetBSD: Makefile,v 1.1 1995/02/28 23:26:21 fvdl Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	linux_sysent.c linux_syscalls.c linux_syscall.h linux_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
