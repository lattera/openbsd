/*
  (c) Copyright 1986 HEWLETT-PACKARD COMPANY
  To anyone who acknowledges that this file is provided "AS IS"
  without any express or implied warranty:
      permission to use, copy, modify, and distribute this file
  for any purpose is hereby granted without fee, provided that
  the above copyright notice and this notice appears in all
  copies, and that the name of Hewlett-Packard Company not be
  used in advertising or publicity pertaining to distribution
  of the software without specific, written prior permission.
  Hewlett-Packard Company makes no representations about the
  suitability of this software for any purpose.
*/
/* $Source: /usr/local/kcs/sys.REL9_05_800/spmath/RCS/mpyu.c,v $
 * $Revision: 1.6.88.1 $	$Author: root $
 * $State: Exp $   	$Locker:  $
 * $Date: 93/12/07 15:06:48 $
 */


#include "../spmath/md.h"

VOID mpyu(opnd1,opnd2,result)

unsigned int opnd1, opnd2;
struct mdsfu_register *result;
{
	impyu(&opnd1,&opnd2,result);
	/* determine overflow status */
	if (result_hi) overflow = TRUE;
	else overflow = FALSE;
	return;
}
