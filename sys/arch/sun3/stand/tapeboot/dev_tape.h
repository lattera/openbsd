/*	$OpenBSD: src/sys/arch/sun3/stand/tapeboot/Attic/dev_tape.h,v 1.2.16.2 2002/03/28 11:26:45 niklas Exp $	*/


int	tape_open(struct open_file *, ...);
int	tape_close(struct open_file *);
int	tape_strategy(void *, int, daddr_t, size_t, void *, size_t *);
int	tape_ioctl();

