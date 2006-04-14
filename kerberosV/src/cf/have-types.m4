dnl
dnl $KTH: have-types.m4,v 1.3 2004/02/12 14:20:52 lha Exp $
dnl

AC_DEFUN([AC_HAVE_TYPES], [
for i in $1; do
        AC_HAVE_TYPE($i)
done
if false;then
	AC_CHECK_FUNCS($1)
fi
])
