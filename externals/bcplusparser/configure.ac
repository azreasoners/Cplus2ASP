#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(cplus2asp, 2.0, jbabb1@asu.edu)
AC_CONFIG_SRCDIR([src/bcplus/Configuration.r2c])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_AUX_DIR([build-scripts])
AC_CONFIG_MACRO_DIR([build-scripts])
m4_include([build-scripts/boost.m4])

# Setup Automake
AM_INIT_AUTOMAKE
AC_SUBST([PROJECT_ROOT],`pwd`)

AC_CONFIG_FILES([Makefile
	src/Makefile lemon/Makefile])

# Allow RE2C and LEMON to build by default
AM_MAINTAINER_MODE([enable])


# Checks for programs.
C_PROG_LIBTOOL 
AC_PROG_CXX
AC_PROG_CC
AC_PATH_PROG([RE2C],[re2c],[false])
AC_SUBST(RE2C)
AM_CONDITIONAL([HAVE_RE2C],[test "x$RE2C" != x"false"])

LT_INIT

BOOST_STATIC()

# Allow portable building
AC_ARG_ENABLE(portable,
AS_HELP_STRING([--enable-portable],
               [enable portable build, default: no]),
[case "${enableval}" in
             yes) \
				portable=true; \
				enable_static_boost=yes; \
  				LDFLAGS="-Wl,-rpath -Wl,\\\$\$ORIGIN/lib $LDFLAGS";;
#				CFLAGS="$CFLAGS -DUSE_RELATIVE_PATHS";;
             no)  \
				portable=false;; 
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-portable]) ;;
esac],
[portable=false])

AC_ARG_ENABLE([relpath],
  AC_HELP_STRING([--enable-relpath], [Data and libraries are searched relative to the executable]),
  [want_relative=yes], [want_relative=no])
# if [ test "$want_relative" == "yes" ]; then
# fi

# BOOST Libraries

if [ test "x$portable" == x"true" ]
then
	rt_type="s"
else
	rt_type=""
fi


 
BOOST_REQUIRE([1.44])

BOOST_STRING_ALGO([$rt_type])
BOOST_SYSTEM([$rt_type])
BOOST_FILESYSTEM([$rt_type])
BOOST_IOSTREAMS([$rt_type])

# Checks for libraries.

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([inttypes.h libintl.h malloc.h stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset realpath strchr strdup])

# Allow Debugging
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

AC_OUTPUT
