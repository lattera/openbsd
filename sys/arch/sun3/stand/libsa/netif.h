/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/netif.h,v 1.2.16.2 2002/03/28 11:26:45 niklas Exp $	*/


#include "iodesc.h"

struct netif {
	void *nif_devdata;
};

ssize_t		netif_get(struct iodesc *, void *, size_t, time_t);
ssize_t		netif_put(struct iodesc *, void *, size_t);

int		netif_open(void *);
int		netif_close(int);

struct iodesc	*socktodesc(int);

