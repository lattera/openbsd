/*	$OpenBSD: src/sys/arch/sun3/stand/netboot/Attic/dev_net.h,v 1.2 2001/07/04 08:33:54 niklas Exp $	*/


int	net_open __P((struct open_file *, ...));
int	net_close __P((struct open_file *));
int	net_ioctl();
int	net_strategy();

