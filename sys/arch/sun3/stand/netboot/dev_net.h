/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/dev_net.h,v 1.3 2002/03/14 01:26:47 millert Exp $	*/


int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

