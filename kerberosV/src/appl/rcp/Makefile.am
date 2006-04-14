# $KTH: Makefile.am,v 1.3 2004/08/26 11:55:18 joda Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4)

bin_PROGRAMS = rcp

rcp_SOURCES  = rcp.c util.c

LDADD = $(LIB_roken)
