#	from: @(#)Makefile.inc	5.21 (Berkeley) 5/24/91
#	$Id: GNUmakefile.inc,v 1.7 1995/03/10 03:58:48 snl Exp $

# gen sources
VPATH:= ${VPATH}:${srcdir}/gen

SRCS:= ttyname.c directory.c popen.c time.c ctime.c difftime.c syslog.c \
	eprintf.c getpwent.c getpwnamuid.c pwd_internal.c \
        getcwd.c getwd.c isatty.c $(SRCS)
