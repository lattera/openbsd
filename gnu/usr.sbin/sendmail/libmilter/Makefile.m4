include(confBUILDTOOLSDIR`/M4/switch.m4')

define(`confREQUIRE_LIBSM', `true')
define(`confMT', `true')

# sendmail dir
SMSRCDIR=	ifdef(`confSMSRCDIR', `confSMSRCDIR', `${SRCDIR}/sendmail')
PREPENDDEF(`confINCDIRS', `-I${SMSRCDIR} ')

bldPRODUCT_START(`library', `libmilter')
define(`bldINSTALLABLE', `true')
define(`LIBMILTER_EXTRAS', `errstring.c strl.c')
define(`bldSOURCES', `main.c engine.c listener.c handler.c comm.c smfi.c signal.c sm_gethost.c LIBMILTER_EXTRAS ')
define(`confBEFORE', `LIBMILTER_EXTRAS')
bldPUSH_INSTALL_TARGET(`install-mfapi')
bldPRODUCT_END
APPENDDEF(`confENVDEF', `-DNOT_SENDMAIL')

PUSHDIVERT(3)
errstring.c:
	${LN} ${LNOPTS} ${SRCDIR}/libsm/errstring.c .

strl.c:
	${LN} ${LNOPTS} ${SRCDIR}/libsm/strl.c .
POPDIVERT

divert(bldTARGETS_SECTION)
# Install the API header file
MFAPI=	${SRCDIR}/inc`'lude/libmilter/mfapi.h
install-mfapi: ${MFAPI}
	${INSTALL} -c -o ${INCOWN} -g ${INCGRP} -m ${INCMODE} ${MFAPI} ${DESTDIR}${INCLUDEDIR}
divert(0)

bldFINISH
