/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/dev_net.h,v 1.1.1.1.16.2 2002/03/28 11:26:45 niklas Exp $	*/


int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

