#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(cplus2asp, 4.0, samidh.talsania@asu.edu)
AC_CONFIG_SRCDIR([src/cplus2asp/utils.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-scripts])
AC_CONFIG_MACRO_DIR([build-scripts])
m4_include([build-scripts/boost.m4])

# Setup Automake
AM_INIT_AUTOMAKE
AC_SUBST([PROJECT_ROOT],`pwd`)

AC_CONFIG_FILES([Makefile
	src/cplus2asp.bin/Makefile
	src/bcbridge/Makefile
	src/cplus2asp/Makefile
	src/clingo3to4/Makefile])

AC_CHECK_FILE(
	[externals/as2transition/configure.ac],
	[AS2TRANS="true"],
	[AS2TRANS="false"]
)

AC_CHECK_FILE(
	[externals/bcplusparser/configure.ac],
	[BCPPARSER="true"],
	[BCPPARSER="false"]
)

if test x"$AS2TRANS" == x"true"
then
	AC_CONFIG_SUBDIRS([externals/as2transition])
else
	AC_MSG_WARN([as2transition was not found in the project directory. Please download as2transition from https://github.com/babb517/as2transition and place the files in the externals/as2transition directory.])
fi

AM_CONDITIONAL(HAVE_AS2TRANSITION,"$AS2TRANS")


AC_CHECK_FILE(
	[externals/bcplusparser/configure.ac],
	[BCPPARSER="true"],
	[BCPPARSER="false"]
)

if test x"$BCPPARSER" == x"true"
then
	AC_CONFIG_SUBDIRS([externals/bcplusparser])
else
	AC_MSG_WARN([bcplusparser was not found in the project directory. Please download bcplusparser from https://github.com/babb517/bcplusparser and place the files in the externals/bcplusparser directory.])
fi

AM_CONDITIONAL(HAVE_BCPLUSPARSER,"$BCPPARSER")


# Allows LEX, YACC, and RE2C to rebuild their files.
AM_MAINTAINER_MODE([enable])

# Checks for programs.
C_PROG_LIBTOOL 
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
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
BOOST_ASIO([$rt_type])							# Required by acplusbridge
BOOST_BIND([$rt_type])							# Required by acplusbridge

BOOST_STRING_ALGO([$rt_type])					# Required by cplus2asp
BOOST_FILESYSTEM([$rt_type])					# Required by cplus2asp
BOOST_FUNCTION([$rt_type])						# Required by acplusbridge
BOOST_IOSTREAMS([$rt_type])						# Required by cplus2asp						
BOOST_THREADS([$rt_type])						# Required by acplusbridge
BOOST_REGEX([$rt_type])							# Required by clingo3to4
BOOST_PROGRAM_OPTIONS([$rt_type])				# Required by clingo3to4

# Static linking appears to be problematic with the boost timers.
if [ test "x$portable" != x"true" ]
then

	BOOST_FIND_HEADER(
		["boost/timer/timer.hpp"], 
		[
			AC_MSG_WARN(["Could not locate the boost timer library. Some benchmarking functions will be disabled."])
			AM_CONDITIONAL([HAVE_BOOST_TIMER],[test "x" == "y" ])
		],
		[
			BOOST_TIMER([$rt_type])
			AM_CONDITIONAL([HAVE_BOOST_TIMER],[test "x" == "x"])
		])
else
	AM_CONDITIONAL([HAVE_BOOST_TIMER],[test "x" == "y" ])
fi


# Other libraries
AC_CHECK_LIB(rt, clock_gettime,[],[AC_MSG_ERROR([Could not find librt, which is required for compilation. Please install librt and try again.])])



#AC_PATH_PROG([ICLINGO],[iclingo],[false])
#AC_PATH_PROG([OCLINGO],[oclingo],[false])
AC_PATH_PROG([CLINGO], [clingo], [false])
AC_PATH_PROG([F2LP],   [f2lp], 	 [false])
AC_PATH_PROG([CLINGO3TO4], [clingo3to4], [false])

#if [ test x"$ICLINGO" == x"false" ]
#then
#	AC_MSG_WARN([Could not find iclingo which is required for the normal operation of cplus2asp. Please install iclingo from http://potassco.sourceforge.net before continuing.])
#fi

#if [ test x"$OCLINGO" == x"false" ]
#then
#	AC_MSG_WARN([Could not find oclingo which is required for using the reactive features of cplus2asp. Please install oclingo from http://potassco.sourceforge.net before attempting to use these features.])
#fi

if [ test x"$CLINGO" == x"false" ]
then
	AC_MSG_WARN([Could not find clingo which is required for the static running modes of cplus2asp. Please install clingo from http://potassco.sourceforge.net before attempting to use these features.])
fi

if [ test x"$F2LP" == x"false" ]
then
	AC_MSG_WARN([Could not find f2lp which is required for the normal operation of cplus2asp. Please install f2lp from http://reasoning.eas.asu.edu/f2lp before continuing.])
fi

if [ test x"$CLINGO3TO4" == x"false" ]
then
	AC_MSG_WARN([Could not find clingo3to4 which is required for the normal operation of cplus2asp. Please install f2lp from http://reasoning.eas.asu.edu/f2lp before continuing.])
fi

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
