/*	$OpenBSD: src/sys/arch/mvme68k/stand/bootst/conf.c,v 1.1.16.1 2001/10/31 03:01:17 nate Exp $	*/
/*	$NetBSD: conf.c,v 1.2 1995/10/17 22:58:17 gwr Exp $	*/

#include <stand.h>
#include <rawfs.h>
#include <dev_tape.h>

struct fs_ops file_system[] = {
	{
		rawfs_open, rawfs_close, rawfs_read,
		rawfs_write, rawfs_seek, rawfs_stat,
	},
};
int nfsys = 1;

struct devsw devsw[] = {
	{ "tape", tape_strategy, tape_open, tape_close, tape_ioctl },
};
int	ndevs = 1;

int debug;
