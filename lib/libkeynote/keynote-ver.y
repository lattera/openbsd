/* $OpenBSD: src/lib/libkeynote/keynote-ver.y,v 1.2 1999/05/31 20:09:59 angelos Exp $ */
/*
 * The author of this code is Angelos D. Keromytis (angelos@dsl.cis.upenn.edu)
 *
 * This code was written by Angelos D. Keromytis in Philadelphia, PA, USA,
 * in April-May 1998
 *
 * Copyright (C) 1998, 1999 by Angelos D. Keromytis.
 *	
 * Permission to use, copy, and modify this software without fee
 * is hereby granted, provided that this entire notice is included in
 * all copies of any software which is or includes a copy or
 * modification of this software. 
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTY. IN PARTICULAR, THE AUTHORS MAKES NO
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE
 * MERCHANTABILITY OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR
 * PURPOSE.
 */
%union {
	struct s {
		char   *string;
	} s;
};
%type <s.string> STRING VSTRING
%token STRING VSTRING EQ
%nonassoc EQ
%start program
%{
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "keynote.h"
#include "header.h"
%}
%%

program: /* Nothing */
       | expr
       | STRING              { if (kn_add_authorizer(sessid, $1) != 0)
				 return keynote_errno;
                               free($1);
                             }

expr: VSTRING EQ STRING      { int i = kn_add_action(sessid, $1, $3, 0);

                               if (i != 0)
				 return i;
			       free($1);
			       free($3);
                             }
    | expr VSTRING EQ STRING { int i = kn_add_action(sessid, $2, $4, 0);

                               if (i != 0)
				 return i;
			       free($2);
			       free($4);
                             }  
%%
void
kverror(char *s)
{}
