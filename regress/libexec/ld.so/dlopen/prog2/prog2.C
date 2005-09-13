/*
 * Public Domain 2003 Dale Rahn
 *
 * $OpenBSD: src/regress/libexec/ld.so/dlopen/prog2/prog2.C,v 1.1.1.1 2005/09/13 20:51:39 drahn Exp $
 */
#include <iostream>
#include "ab.h"
BB BBmain("main");

int a;
int
main()
{
	std::cout << "main\n";
	return 0;
}
