/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/dev_disk.h,v 1.1.1.1.16.2 2002/03/28 11:26:45 niklas Exp $	*/


int	disk_open(struct open_file *, ...);
int	disk_close(struct open_file *);
int	disk_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	disk_ioctl();

