#	$Sendmail: HP-UX.11.x,v 8.23 2001/05/30 00:03:35 ca Exp $

# +z is to generate position independant code
define(`confCClibsmi', `cc -Ae +Z')
define(`confCC', `cc -Ae')
define(`confMAPDEF', `-DNDBM -DNIS -DMAP_REGEX')
define(`confENVDEF', `-DV4FS -DHPUX11')
define(`confSM_OS_HEADER', `sm_os_hp')
define(`confOPTIMIZE', `+O3')
define(`confLIBS', `-ldbm -lnsl')
define(`confSHELL', `/usr/bin/sh')
define(`confINSTALL', `${BUILDBIN}/install.sh')
define(`confSBINGRP', `mail')
define(`confEBINDIR', `/usr/sbin')

define(`confMTCCOPTS', `-D_POSIX_C_SOURCE=199506L +z')
define(`confMTLDOPTS', `-lpthread')
define(`confLD', `ld')
define(`confLDOPTS_SO', `-b')
define(`confCCOPTS_SO', `')
