#include "includes.h"
RCSID("$Id: ssh.c,v 1.24 1999/10/14 18:17:42 markus Exp $");

#include "ssh.h"

int compat13=0;
void enable_compat13(void){
	log("Enabling compatibility mode for protocol 1.3");
	compat13=1;
}
