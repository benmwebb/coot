

# AM_PATH_MMDB([ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])

AC_DEFUN([AM_WITH_PYTHON], 
[AC_PROVIDE([AM_USE_PYTHON])

AC_MSG_CHECKING([for Python])


AC_ARG_WITH(python, [  --with-python=PFX Prefix where PYTHON has been installed],
 with_python_prefix="$withval",
 with_python_prefix="")


saved_LIBS="$LIBS"
saved_CFLAGS="$CFLAGS"

if test x$with_python != x; then

   #  ;^)
   #	

   # First check to see if python exists
   #
   python -c ''
   if test $? != 0 ; then
      echo python not found. 
      # This is an error because it was explicitly asked for
      # AC_MSG_ERROR([--with-python was specified, but no python was found])
	echo error
   fi
   
   # normal execution proceeds..
   PYTHON_CFLAGS="-DUSE_PYTHON -I`python -c 'import sys; print sys.prefix + "/include/python" + sys.version[[:3]]'`"
   # PYTHON_LIBS="-L/h/paule/build/lib/python2.2/config -lpython2.2 -lutil"
   PYTHON_LIBS_PRE="-L`python -c 'import sys; print sys.prefix + "/lib/python" + sys.version[[:3]] + "/config"'` -lpython`python -c 'import sys; print sys.version[[:3]]'`"
	
   # now we have to deal with the -lutil issue.  On GNU/Linux, we need
   # it, MacOS X we don't (it doesn't exist and the compliation fails)
   # Cygwin is like MacOS X.
   # 
   # Recall that for RedHat, uname returns "Linux"
   #
   # consider using AM_CONDITIONAL, see the automake manual.
   # consider using config.guess output for python util check
   # config.guess returns e.g. i686-pc-linux-gnu, powerpc-apple-darwin7.0.0
   #
   UTIL_LIB="-lutil"
   py_uname=`uname`
   case	"$py_uname" in
	Darwin)
		UTIL_LIB=""
	;; 
	Cygwin*|CYGWIN*|cygwin*)
		UTIL_LIB=""
	;;
   esac	

   PYTHON_LIBS="$PYTHON_LIBS_PRE $UTIL_LIB"
   AC_MSG_RESULT(yes)
   echo Cool, using Python
   coot_python=true	

else 

   AC_MSG_RESULT(no)
   echo Not using python
   PYTHON_CFLAGS=""
   PYTHON_LIBS=""
   # COOT_WRAP_PYTHON_CONVERT="cp"
   coot_python=false

fi

AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_LIBS)

AM_CONDITIONAL(COOT_USE_PYTHON, test x$coot_python = xtrue)
])

dnl Stuart McNicholas writes:
dnl Yes, but make sure python is in your path.

dnl echo "import sys;print sys.prefix + '/include/python' + sys.version[:3]" | 
dnl python

dnl and 
dnl echo "import sys;print '-L' + sys.prefix + '/lib/python' + sys.version[:3] + 
dnl '/config' + ' -lpython' + sys.version[:3]" | python

