/*	$OpenBSD: src/sys/arch/sun3/dev/Attic/zs_cons.h,v 1.1.24.1 2002/06/11 03:39:01 art Exp $	*/

extern void *zs_conschan;

extern void nullcnprobe(struct consdev *);

extern int  zs_getc(void *arg);
extern void zs_putc(void *arg, int c);

