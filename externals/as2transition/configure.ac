#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(as2transition, 3.1, jbabb1@asu.edu)
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-scripts])


# Setup Automake
AM_INIT_AUTOMAKE
AC_SUBST([PROJECT_ROOT],`pwd`)
AC_CONFIG_FILES([Makefile src/Makefile lemon/Makefile])


# Allows LEX, YACC, and RE2C to rebuild their files.
AM_MAINTAINER_MODE([enable])

# Checks for programs.
C_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AC_PATH_PROG([RE2C],[re2c],[false])
AC_SUBST(RE2C)
AM_CONDITIONAL([HAVE_RE2C],[test "x$RE2C" != x"false"])

LT_INIT

# Checks for libraries.

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T


# Allow portable building
AC_ARG_ENABLE(portable,
AS_HELP_STRING([--enable-portable],
               [enable portable build, default: no]),
[case "${enableval}" in
             yes) \
                portable=true; \
                LDFLAGS="-Wl,-rpath -Wl,\\\$\$ORIGIN/lib $LDFLAGS";;
#               CFLAGS="$CFLAGS -DUSE_RELATIVE_PATHS";;
             no)  \
                portable=false;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-portable]) ;;
esac],
[portable=false])


AC_ARG_ENABLE([relpath],
  AC_HELP_STRING([--enable-relpath], [Data and libraries are searched relative to the executable]),
  [want_relative=yes], [want_relative=no])

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
