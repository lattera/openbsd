/*	$OpenBSD: src/sys/arch/sun3/sun3/Attic/ledsvar.h,v 1.2.24.1 2002/06/11 03:39:01 art Exp $	*/

extern volatile unsigned int led_n_patterns;
extern volatile unsigned int led_countmax;
extern volatile const unsigned char * volatile led_patterns;
extern volatile unsigned int led_countdown;
extern volatile unsigned int led_px;

extern int ledrw(struct uio *);
