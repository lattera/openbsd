# $Id: Makefile.in,v 1.12 1997/07/18 22:32:08 assar Exp $

SHELL = /bin/sh

MAKEINFO = /usr/bin/makeinfo
INSTALL-INFO = /usr/bin/install-info

infodir = ${DESTDIR}/usr/share/info

INSTALL = /usr/bin/install

info = kth-krb.info

SRCS = 	ack.texi	\
	index.texi	\
	intro.texi	\
	kth-krb.texi	\
	otp.texi	\
	problems.texi	\
	setup.texi	\
	whatis.texi


all: $(info)

install: $(info)
	$(INSTALL-INFO) ${.OBJDIR}/$(info) $(infodir)/dir 
	$(INSTALL) -m 444 -o root -g wheel -p ${.OBJDIR}/$(info) $(infodir)

$(info): $(SRCS)
	$(MAKEINFO) -I${.CURDIR} -o ${.OBJDIR}/$@ ${.CURDIR}/kth-krb.texi

clean:
	(cd ${.OBJDIR} ; rm -f *.aux *.cp *.cps *.dvi *.fn *.ky *.log *.pg *.toc *.tp *.vr *.info)

cleandir: clean


.PHONY: install all clean cleandir

.include <bsd.obj.mk>
.include <bsd.subdir.mk>
