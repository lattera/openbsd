/*
 * Public Domain 2011 Kurt Miller
 *
 * $OpenBSD: src/regress/libexec/ld.so/initfirst/test2/libad/ad.C,v 1.1 2011/11/29 04:36:15 kurt Exp $
 */

#include <iostream>

class AD
{
	public:
		AD();
		~AD();
};

AD::AD()
{
   std::cout << "D";
}

AD::~AD()
{
   std::cout << "d";
}

AD d;
