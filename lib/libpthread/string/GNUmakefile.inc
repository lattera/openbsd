#	from: @(#)Makefile.inc	5.21 (Berkeley) 5/24/91
#	$Id: GNUmakefile.inc,v 1.1 1995/09/20 00:46:14 ghudson Exp $

# gen sources
VPATH:= ${VPATH}:${srcdir}/string

SRCS:= strtok.c $(SRCS)
