/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/dev_net.h,v 1.2.4.1 2002/06/11 03:39:01 art Exp $	*/


int	net_open(struct open_file *, ...);
int	net_close(struct open_file *);
int	net_ioctl();
int	net_strategy();

