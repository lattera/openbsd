dnl
dnl $Id: try-compile-kernel.m4,v 1.2 2000/02/22 23:01:53 assar Exp $
dnl

AC_DEFUN(AC_TRY_COMPILE_KERNEL,[
save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $test_KERNEL_CFLAGS $KERNEL_CPPFLAGS"
AC_TRY_COMPILE([$1], [$2], [$3], [$4])
CFLAGS="$save_CFLAGS"
])
