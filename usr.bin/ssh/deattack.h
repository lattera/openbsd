/* $Id: deattack.h,v 1.2 1998/06/11 12:03:56 kivinen Exp $
 * Cryptographic attack detector for ssh - Header file
 * (C)1998 CORE-SDI, Buenos Aires Argentina
 * Ariel Futoransky(futo@core-sdi.com)
 */

#ifndef _DEATTACK_H
#define _DEATTACK_H

/* Return codes */
#define DEATTACK_OK		0
#define DEATTACK_DETECTED	1

int detect_attack(unsigned char *buf, u_int32_t len, unsigned char IV[8]);
#endif
