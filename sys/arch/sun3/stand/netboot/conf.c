/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/conf.c,v 1.2.16.1 2001/10/31 03:08:00 nate Exp $	*/
/*	$NetBSD: conf.c,v 1.3 1996/01/29 23:54:14 gwr Exp $	*/

#include <sys/types.h>
#include <netinet/in.h>

#include "stand.h"
#include "nfs.h"
#include "dev_net.h"

struct fs_ops file_system[] = {
	{ nfs_open, nfs_close, nfs_read, nfs_write, nfs_seek, nfs_stat },
};
int nfsys = 1;

struct devsw devsw[] = {
	{ "net",  net_strategy,  net_open,  net_close,  net_ioctl },
};
int	ndevs = 1;

