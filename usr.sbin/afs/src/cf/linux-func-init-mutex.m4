dnl
dnl $Id: linux-func-init-mutex.m4,v 1.6 2000/02/22 23:01:53 assar Exp $
dnl

AC_DEFUN(AC_LINUX_FUNC_INIT_MUTEX, [
AC_CACHE_CHECK([for init_MUTEX],
ac_cv_func_init_mutex, [
save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $test_KERNEL_CFLAGS $KERNEL_CPPFLAGS"
AC_EGREP_CPP([init_MUTEX],
[#include <asm/semaphore.h>],
ac_cv_func_init_mutex=yes,
ac_cv_func_init_mutex=no)]
CPPFLAGS="$save_CPPFLAGS"
)
if test "$ac_cv_func_init_mutex" = "yes"; then
  AC_DEFINE(HAVE_INIT_MUTEX, 1,
	[define if you have a function init_MUTEX])
fi
])
