#----------------------------------------------------------------------------
# Autoconf input script. Invoke the ./autogen.sh script to generate a
# configure script from this file.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

#----------------------------------------------------------------------------
# Initialize Autoconf.
#----------------------------------------------------------------------------
AC_INIT(
    [bullet],
    [2.0],
    [bullet@erwincoumans.com])
CS_PACKAGEINFO(
    [Bullet Continuous Collision Detection and Physics Library],
    [Copyright (c) 2005-2006 Erwin Coumans],
    [http://www.continuousphysics.com/Bullet/])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])
AC_CANONICAL_HOST

#----------------------------------------------------------------------------
# Setup for the configuration header.
#----------------------------------------------------------------------------
AC_CONFIG_HEADERS([config.h])

#----------------------------------------------------------------------------
# Check for tools.
#----------------------------------------------------------------------------
CS_PROG_CC
AS_IF([test -z "$CC"],
    [AC_MSG_ERROR([Could not find a usable C compiler.])])
CS_PROG_CXX
AS_IF([test -z "$CXX"],
    [AC_MSG_ERROR([Could not find a usable C++ compiler.])])
CS_PROG_LINK

CS_CHECK_COMMON_TOOLS_LINK
CS_CHECK_COMMON_TOOLS_BASIC
CS_CHECK_COMMON_TOOLS_DOC_DOXYGEN

CS_CHECK_PROGS([PERL], [perl5 perl])
CS_EMIT_BUILD_PROPERTY([PERL], [$PERL])

CS_CHECK_TEMPLATE_TOOLKIT2([emit])

#----------------------------------------------------------------------------
# Check if C++ exceptions can be disabled.
#----------------------------------------------------------------------------
CS_EMIT_BUILD_FLAGS([how to disable C++ exceptions],
    [cs_cv_prog_cxx_disable_exceptions], [CS_CREATE_TUPLE([-fno-exceptions])],
    [C++], [COMPILER.C++FLAGS.EXCEPTIONS.DISABLE], [],
    [CS_EMIT_BUILD_PROPERTY([COMPILER.C++FLAGS],
	[$cs_cv_prog_cxx_disable_exceptions], [+])])


#----------------------------------------------------------------------------
# Determine system type
#----------------------------------------------------------------------------
CS_CHECK_HOST

#----------------------------------------------------------------------------
# Check for syntax problems / header files
#----------------------------------------------------------------------------
# Nothing yet.

#----------------------------------------------------------------------------
# Check for GLUT.
#----------------------------------------------------------------------------
AS_IF([test $cs_host_family = windows],
    [# Tack the GLUT that comes with bullet onto compiler & linker flags.
    _AC_SRCPATHS(["."])
    glut_cflags="-I$ac_top_srcdir/Glut"
    glut_lflags="-L$ac_top_srcdir/Glut"
    CFLAGS="$CFLAGS $glut_cflags"
    LDFLAGS="$LDFLAGS $glut_lflags"
    CS_EMIT_BUILD_PROPERTY([COMPILER.CFLAGS], [$glut_cflags], [+])
    CS_EMIT_BUILD_PROPERTY([COMPILER.LFLAGS], [$glut_lflags], [+])
    ])
CS_CHECK_GLUT

#----------------------------------------------------------------------------
# Package configuration switches.
#----------------------------------------------------------------------------
AC_ARG_ENABLE([debug],
    [AC_HELP_STRING([--enable-debug],
	[build with debugging information (default NO)])],
    [], [enable_debug=no])

AC_MSG_CHECKING([build mode])
AS_IF([test $enable_debug = yes], [build_mode=debug], [build_mode=optimize])
AC_MSG_RESULT([$build_mode])

CS_EMIT_BUILD_PROPERTY([MODE], [$build_mode])

#-----------------------------------------------------------------------------
# Emit install paths and package information.
#-----------------------------------------------------------------------------
CS_OUTPUT_INSTALLDIRS
CS_EMIT_PACKAGEINFO

#----------------------------------------------------------------------------
# Emit generated files.
#----------------------------------------------------------------------------
CS_JAMCONFIG_OUTPUT([Jamconfig])
AC_CONFIG_FILES([Jamfile bullet.pc])
AC_OUTPUT

AC_MSG_NOTICE([

Please note that this project uses Jam as its build tool,
therefore you must type 'jam' to build the project.

Jam home: http://www.perforce.com/jam/jam.html
Jam source: ftp://ftp.perforce.com/jam/
])
