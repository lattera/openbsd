#ifndef lint
static char rcsid[] = "$Id: infinity.c,v 1.2 1995/11/11 21:05:00 build Exp $";
#endif /* not lint */

/* infinity.c */

#include <math.h>

/* bytes for +Infinity on 88100 */
char __infinity[] = { 0x7f, 0xf0, 0, 0, 0, 0, 0, 0 };
