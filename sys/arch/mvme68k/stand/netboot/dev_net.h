/*	$OpenBSD: src/sys/arch/mvme68k/stand/netboot/dev_net.h,v 1.2.16.2 2004/02/19 10:49:05 niklas Exp $ */

int	net_open(struct open_file *, char *);
int	net_close(struct open_file *);
int	net_ioctl(struct open_file *, u_long, void *);
int	net_strategy(void *, int, daddr_t, size_t, void *, size_t *);

void	machdep_common_ether(u_char *);
