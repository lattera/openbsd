/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/dev_disk.h,v 1.2.4.1 2002/06/11 03:39:01 art Exp $	*/


int	disk_open(struct open_file *, ...);
int	disk_close(struct open_file *);
int	disk_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	disk_ioctl();

