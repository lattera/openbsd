/*	$OpenBSD: src/sys/netiso/xebec/Attic/debug.h,v 1.2.16.1 2001/10/31 03:29:04 nate Exp $	*/
/*	$NetBSD: debug.h,v 1.4 1994/06/29 06:41:00 cgd Exp $	*/

#define OUT stdout

extern int	debug[128];

#ifdef DEBUG
extern int column;

#define IFDEBUG(letter) \
	if(debug['letter']) { 
#define ENDDEBUG  ; (void) fflush(stdout);}

#else 

#define STAR *
#define IFDEBUG(letter)	 //*beginning of comment*/STAR
#define ENDDEBUG	 STAR/*end of comment*//

#endif /* DEBUG */

