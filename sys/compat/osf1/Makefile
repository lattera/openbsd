#	$OpenBSD: src/sys/compat/osf1/Attic/Makefile,v 1.2.16.2 2001/10/31 03:11:46 nate Exp $
#	$NetBSD: Makefile,v 1.1 1995/02/13 21:39:00 cgd Exp $

DEP=	syscalls.conf syscalls.master ../../kern/makesyscalls.sh
OBJS=	osf1_sysent.c osf1_syscalls.c osf1_syscall.h osf1_syscallargs.h

${OBJS}: ${DEP}
	sh ../../kern/makesyscalls.sh syscalls.conf syscalls.master

clean:
	rm -f ${OBJS}
