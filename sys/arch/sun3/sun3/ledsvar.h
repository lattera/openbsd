/*	$OpenBSD: src/sys/arch/sun3/sun3/Attic/ledsvar.h,v 1.2.14.1 2002/03/28 11:26:45 niklas Exp $	*/

extern volatile unsigned int led_n_patterns;
extern volatile unsigned int led_countmax;
extern volatile const unsigned char * volatile led_patterns;
extern volatile unsigned int led_countdown;
extern volatile unsigned int led_px;

extern int ledrw(struct uio *);
