/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/dev_disk.h,v 1.1.1.1.16.1 2001/10/31 03:08:00 nate Exp $	*/


int	disk_open __P((struct open_file *, ...));
int	disk_close __P((struct open_file *));
int	disk_strategy __P((void *, int, daddr_t, size_t, void *, size_t *));
int	disk_ioctl();

