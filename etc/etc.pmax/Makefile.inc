#
#	etc.pmax/Makefile.inc -- pmax-specific etc Makefile targets
#
#	$Id: Makefile.inc,v 1.3 1996/06/11 10:20:33 deraadt Exp $

.ifdef DESTDIR

snap_md: binutils bsd

binutils:
	cd ${DESTDIR} && tar cf - usr/*openbsd* \
	    | ${SNAP_GZIP_CMD} > snapshot/usr.binutils.tar${SNAP_GZIP_EXT}

bsd:
	cd ${.CURDIR}/../sys/arch/pmax/conf && config GENERIC
	cd ${.CURDIR}/../sys/arch/pmax/compile/GENERIC && \
	    make clean && make depend && make
	cp ${.CURDIR}/../sys/arch/pmax/compile/GENERIC/bsd \
	    ${DESTDIR}/snapshot/bsd

.endif	# DESTDIR check
#
#	etc.alpha/Makefile.inc -- alpha-specific etc Makefile targets
#
#	$Id: Makefile.inc,v 1.3 1996/06/11 10:20:33 deraadt Exp $

.ifdef DESTDIR
# XXX - bsd will come then we have a GENERIC kernel
snap_md: binutils

binutils:
	cd ${DESTDIR} && tar cf - usr/*openbsd* \
	    | ${SNAP_GZIP_CMD} > snapshot/usr.binutils.tar${SNAP_GZIP_EXT}

bsd:
	cd ${.CURDIR}/../sys/arch/alpha/conf && config GENERIC
	cd ${.CURDIR}/../sys/arch/alpha/compile/GENERIC && \
	    make clean && make depend && make
	cp ${.CURDIR}/../sys/arch/alpha/compile/GENERIC/bsd \
	    ${DESTDIR}/snapshot/bsd

.endif	# DESTDIR check
