/*	$OpenBSD: src/sys/arch/sun3/dev/Attic/zs_cons.h,v 1.1.14.1 2002/03/28 11:26:45 niklas Exp $	*/

extern void *zs_conschan;

extern void nullcnprobe(struct consdev *);

extern int  zs_getc(void *arg);
extern void zs_putc(void *arg, int c);

