#	$OpenBSD: src/sys/compat/sunos/Attic/Makefile,v 1.3.4.1 2001/10/31 03:11:46 nate Exp $
#	$NetBSD: Makefile,v 1.1 1996/05/05 16:07:42 veego Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	sunos_sysent.c sunos_syscalls.c sunos_syscall.h sunos_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
