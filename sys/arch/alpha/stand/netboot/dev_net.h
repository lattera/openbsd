/*	$OpenBSD: src/sys/arch/alpha/stand/netboot/dev_net.h,v 1.1.24.1 2002/06/11 03:33:41 art Exp $	*/


int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

