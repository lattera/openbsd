/*	$OpenBSD: src/sys/arch/sun3/stand/tapeboot/Attic/dev_tape.h,v 1.2.16.1 2001/10/31 03:08:00 nate Exp $	*/


int	tape_open __P((struct open_file *, ...));
int	tape_close __P((struct open_file *));
int	tape_strategy __P((void *, int, daddr_t, size_t, void *, size_t *));
int	tape_ioctl();

