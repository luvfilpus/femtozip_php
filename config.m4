dnl $Id$
dnl config.m4 for extension fzip

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(fzip, for fzip support,
dnl Make sure that the comment is aligned:
[  --with-fzip             Include fzip support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(fzip, whether to enable fzip support,
dnl Make sure that the comment is aligned:
dnl [  --enable-fzip           Enable fzip support])

if test "$PHP_FZIP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-fzip -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="femtozip.h"  # you most likely want to change this
  if test -r $PHP_FZIP/$SEARCH_FOR; then # path given as parameter
  	FZIP_DIR=$PHP_FZIP
  else # search default path list
  	AC_MSG_CHECKING([for fzip files in default path])
  	for i in $SEARCH_PATH ; do
       if test -r $i/include/$SEARCH_FOR; then
         FZIP_DIR=$i
         AC_MSG_RESULT(found in $i)
       fi
     done
  fi
  dnl
  if test -z "$FZIP_DIR"; then
  	AC_MSG_RESULT([not found])
  	AC_MSG_ERROR([Please reinstall the fzip distribution])
  fi

  dnl # --with-fzip -> add include path
  dnl PHP_ADD_INCLUDE($FZIP_DIR/include)

  dnl # --with-fzip -> check for lib and symbol presence
  LIBNAME=fzip # you may want to change this
  LIBSYMBOL=fz_compress # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FZIP_DIR/lib, FZIP_SHARED_LIBADD)
    AC_DEFINE(HAVE_FZIPLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong fzip lib version or lib not found])
  ],[
    -L$FZIP_DIR/lib -lz
  ])
  
  PHP_SUBST(FZIP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(fzip, fzip.c, $ext_shared)

fi
