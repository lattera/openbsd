/*	$OpenBSD: src/sys/arch/sun3/stand/libsa/Attic/promdev.h,v 1.2 2001/07/04 08:33:52 niklas Exp $	*/


int  prom_iopen (struct saioreq *sip);
void prom_iclose(struct saioreq *sip);

