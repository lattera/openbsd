/*	$OpenBSD: src/sys/arch/sun3/stand/tapeboot/Attic/dev_tape.h,v 1.3.4.1 2002/06/11 03:39:01 art Exp $	*/


int	tape_open(struct open_file *, ...);
int	tape_close(struct open_file *);
int	tape_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	tape_ioctl();

