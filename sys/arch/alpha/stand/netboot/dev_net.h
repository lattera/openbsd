/*	$OpenBSD: src/sys/arch/alpha/stand/netboot/dev_net.h,v 1.1.14.1 2002/03/28 10:06:13 niklas Exp $	*/


int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

