/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/dvma.h,v 1.1.1.1.16.1 2001/10/31 03:08:00 nate Exp $	*/


void dvma_init();

char * dvma_mapin(char *pkt, int len);
void dvma_mapout(char *dmabuf, int len);

char * dvma_alloc(int len);
void dvma_free(char *dvma, int len);

