/*
 * Public Domain 2003 Dale Rahn
 *
 * $OpenBSD: src/regress/libexec/ld.so/dlopen/libac/ac.h,v 1.1 2005/09/17 02:58:54 drahn Exp $
 */

class AC {
public:
        AC(char *);
        ~AC();
private:
	char *_name;
};

