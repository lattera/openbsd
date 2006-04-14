dnl $KTH: krb-prog-ranlib.m4,v 1.2 2004/02/12 14:20:58 lha Exp $
dnl
dnl
dnl Also look for EMXOMF for OS/2
dnl

AC_DEFUN([AC_KRB_PROG_RANLIB],
[AC_CHECK_PROGS(RANLIB, ranlib EMXOMF, :)])
