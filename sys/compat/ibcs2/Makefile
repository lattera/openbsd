#	$OpenBSD: src/sys/compat/ibcs2/Attic/Makefile,v 1.2.14.1 2001/10/31 03:11:46 nate Exp $
#	$NetBSD: Makefile,v 1.1 1996/09/03 03:18:12 mycroft Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	ibcs2_sysent.c ibcs2_syscalls.c ibcs2_syscall.h ibcs2_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master
