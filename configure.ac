dnl Process this file with autoconf to produce a configure script.
dnl
VERSIONINFO=""
AC_PREREQ(2.60)
AC_INIT([mvwm], [2.6.6], [thomas@xteddy.org])
AC_CONFIG_AUX_DIR(etc)
AC_CONFIG_HEADER(config.h)

dnl should be "yes" only within the released distribution
ISRELEASED=no

dnl date of the released version (please zero pad the day in the last 2 dates)
dnl for example: "4 February 2003", "04 Feb 2003", "2003-02-04"
dnl date format strings: "%e %B %Y", "%d-%b-%Y", "%Y-%m-%d"
RELDATELONG="20 April 2012"
RELDATESHORT="02-Apr-2012"
RELDATENUM="2012-04-20"

# constant variable settings
MVWMNAMELONG="F? Virtual Window Manager"
MVWMHOMEPAGE="http://xteddy.org/"
MVWMFTP=""
MVWMFTPDIR=""
MVWMALTFTP=""
MVWMALTFTPDIR=""
MVWMLIST="thomas@xteddy.org"
MVWMWORKERSLIST="thomas@xteddy.org"
MVWMWORKERSLISTLONG="Thomas Adam <thomas@xteddy.org>"
MANPAGE_PREAMBLE='.\" WARNING: This file was automatically generated.  Edit the .in file instead.'

if test ! x"$ISRELEASED" = xyes; then
	VERSIONINFO=" (from git)"
	RELDATELONG="(not released yet)"
	RELDATESHORT="(not released yet)"
	RELDATENUM="(not released yet)"
fi

AC_SUBST(ISRELEASED)
AH_TEMPLATE([VERSIONINFO],[Additional version information, like date])
AC_DEFINE_UNQUOTED(VERSIONINFO, "$VERSIONINFO")
AC_SUBST(VERSIONINFO)
AC_SUBST(RELDATELONG)
AC_SUBST(RELDATESHORT)
AC_SUBST(RELDATENUM)
AC_SUBST(MVWMNAMELONG)
AC_SUBST(MVWMHOMEPAGE)
AC_SUBST(MVWMFTP)
AC_SUBST(MVWMFTPDIR)
AC_SUBST(MVWMALTFTP)
AC_SUBST(MVWMALTFTPDIR)
AC_SUBST(MVWMLIST)
AC_SUBST(MVWMWORKERSLIST)
AC_SUBST(MVWMWORKERSLISTLONG)
AC_SUBST(MANPAGE_PREAMBLE)

AM_INIT_AUTOMAKE([foreign])

# check for programs needed to build html docs
AC_CHECK_PROG(SED, sed, sed, "")
AC_SUBST(SED)

# optional python:
AM_PATH_PYTHON([2.5],, [:])

#!!!
PERL=""
REQUIRED_PERL_VERSION=5.004
AC_SUBST(REQUIRED_PERL_VERSION)
if test x"$PERL" = x; then
	AC_PATH_PROG(PERL, perl)
fi
if test ! x"$PERL" = x; then
	AC_MSG_CHECKING([for perl $REQUIRED_PERL_VERSION or better])
	if ("$PERL" -e "require $REQUIRED_PERL_VERSION;") 2>/dev/null; then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_WARN([$PERL not found or too old])
	fi
else
	AC_MSG_WARN([perl is not found, it is required for some scripts and modules])
	AC_MSG_WARN([It is recommended to install perl $REQUIRED_PERL_VERSION or better later])
	PERL=/usr/bin/perl
fi
AC_MSG_RESULT([assuming $PERL as perl location])
AC_SUBST(PERL)

# installation paths
MVWM_MODULESUBDIR=/${PACKAGE}/${VERSION}
MVWM_DATASUBDIR=/${PACKAGE}
MVWM_DOCSUBDIR=/doc/${PACKAGE}

AC_ARG_ENABLE(package-subdirs,
  AS_HELP_STRING([--disable-package-subdirs],
    [do not create subdirs for modules and data]),
  [if test x"$enableval" = xno; then MVWM_MODULESUBDIR=""; MVWM_DATASUBDIR="";
  fi], [])


MVWM_MODULEDIR='${libexecdir}'"$MVWM_MODULESUBDIR"
MVWM_DATADIR='${datadir}'"$MVWM_DATASUBDIR"
MVWM_DOCDIR='${datadir}'"$MVWM_DOCSUBDIR"
MVWM_PERLLIBDIR='${datadir}'"$MVWM_DATASUBDIR/perllib"
MVWM_CONFDIR='${sysconfdir}'dnl used _only_ to search for system.mvwm2rc

AC_SUBST(MVWM_MODULEDIR)
AC_SUBST(MVWM_DATADIR)
AC_SUBST(MVWM_PERLLIBDIR)
AC_SUBST(MVWM_CONFDIR)

AH_TEMPLATE([MVWM_CONFIG],
  [Name of config filenames in MVWM_USERDIR and MVWM_DATADIR])
AC_DEFINE(MVWM_CONFIG, "config")

AH_TEMPLATE([MVWM2RC],[Suffix for old (to be deprecated) config filenames])
AC_DEFINE(MVWM2RC, ".mvwm2rc")

# Various configure-time options

smr_SWITCH(command-log, command logging, off, MVWM_COMMAND_LOG,, [Produces a log of all executed commands and their times on stderr.])
AH_VERBATIM([_MVWM_COMMAND_LOG],
[#ifdef MVWM_COMMAND_LOG
#   define MVWM_DEBUG_TIME 1
#endif])

smr_SWITCH(debug-msgs, debugging messages, off, MVWM_DEBUG_MSGS,, [if you would like to see lots of debug messages from mvwm, for debugging
   purposes, uncomment the next line])
AH_VERBATIM([_MVWM_DEBUG_MSGS],
[#ifdef MVWM_DEBUG_MSGS
#   define DBUG(x,y) mvwm_msg(DBG,x,y)
#else
#   define DBUG(x,y) /* no messages */
#endif])

dnl dummy: smr_SWITCH(sm, dummy for test script, on, SESSION)

# Need to know where X is, for finding some libraries (e.g. xpm)
no_x=""
AC_PATH_XTRA
if test x"$no_x" = x"yes"; then
  echo
  echo "X11 libraries or header files could not be found.  Please make"
  echo "sure the X11 development package is installed on your system."
  echo "If it is definitely installed, try setting the include and library"
  echo "paths with the --x-include and --x-libraries options of configure."
  echo "Mvwm can not be compiled without the X11 development environment."
  echo
  echo "Aborting."
  echo
  exit 1
fi

# FIXME: default value should be derived from computed path to X
# includes.  Actually, this should probably not appear in configure
# at all: it is settable at runtime, and only confuses the issue to
# have it settable here too.
#
AC_MSG_CHECKING(imagepath)
val="/usr/include/X11/bitmaps:/usr/include/X11/pixmaps"
AC_ARG_WITH(imagepath,
  AS_HELP_STRING([--with-imagepath=PATH],
    [colon-delimited search path for images]),
[ case "$withval" in
  no)
    AC_MSG_ERROR([Can not disable image path.])
    ;;
  yes)
    ;;
  *)
    val="$withval"
    ;;
  esac ])
AH_TEMPLATE([MVWM_IMAGEPATH],[Where to search for images.])
AC_DEFINE_UNQUOTED(MVWM_IMAGEPATH, "$val")

MVWM_IMAGEPATH="$val"
AC_SUBST(MVWM_IMAGEPATH)
AC_MSG_RESULT($val)

# Minimal checks for programs: enough to enable checking for
# optional libraries.
AC_PROG_CC
AC_PROG_CPP

# added -Wall for gcc, what about for others?
if test "x$GCC" = "xyes"; then
  CFLAGS="-Wall -Wno-implicit-int $CFLAGS"
fi

# Help finding POSIX functions on some systems
AC_ISC_POSIX
AC_MINIX

# catch -Werror and similar options when running configure
AC_TRY_COMPILE([#include <stdio.h>],
[int i; static j; int *p; char *c;
  switch (*p = p = *c) { case 0: printf("%Q", c, p); }
  *c = &i; c = p;
  while (1 || (unsigned int)3 >= 0 || ((int)-1) == ((unsigned int)1));
  ], , AC_MSG_ERROR("
configure is not able to compile programs with warnings.  Please
remove all offending options like -Werror from the CFLAGS and
CPPFLAGS variables and run configure again."))

# check size of some types
ac_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $X_CFLAGS"
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(Window, , [#include <stdio.h>
#include <X11/X.h>])
AC_CHECK_SIZEOF(Pixel, , [#include <stdio.h>
#include <X11/Intrinsic.h>])
AC_CHECK_SIZEOF(void *)
CFLAGS="$ac_save_CFLAGS"
size_check_failed=""
if test "$ac_cv_sizeof_void_p" -gt "$ac_cv_sizeof_long"; then
  echo "The type void * is bigger than long."
  size_check_failed=1
fi
if test "$ac_cv_sizeof_Window" -gt "$ac_cv_sizeof_long"; then
  echo "The type Window is bigger than long."
  size_check_failed=1
fi
if test "$ac_cv_sizeof_Pixel" -gt "$ac_cv_sizeof_long"; then
  echo "The type Pixel is bigger than long."
  size_check_failed=1
fi
if test x"$size_check_failed" = x1; then
  echo "This is not supported by the module interface yet (make_new_vpacket)."
  echo "Detected type sizes are: int($ac_cv_sizeof_int), long($ac_cv_sizeof_long), void*($ac_cv_sizeof_void_p), Window($ac_cv_sizeof_Window), Pixel($ac_cv_sizeof_Pixel)"
  echo "Please report details of your system and this message to ${MVWMWORKERSLIST}."
  echo
  exit 1
fi

# 'unset' is not portable, but setting to null is not enough to avoid using
# the cached value!  For ancient shells "rm config.cache" is a solution.
UNSET=true
if unset UNSET 2>/dev/null; then UNSET=unset; fi


# ********* multibyte
# FreeBSD has libxpg4, check this and use if found.
AC_CANONICAL_HOST
case $host_os in
freebsd*)
	AC_CHECK_LIB(xpg4, setlocale, [LIBS="$LIBS -lxpg4"]);;
*)
	;;
esac

# *** pkg-config
# unfortunately, we need pkg-config for the detection of certain libs:
# - version of fontconfig without fontconfig-config
# - version of fribidi without fribidi-config
AM_CHECK_PKG_CONFIG

# Checking for optional libraries
# Default is to use them if found; can be disable using --without
# These are put up front so that if they are requested, but
# configure fails to find them, we fail early.


# ********* shape extension
dnl smr_SWITCH(shape, shape extensions, on, SHAPE)
AC_ARG_ENABLE(shape,
  AS_HELP_STRING([--disable-shape],[disable shaped window support]),
  [ if test x"$enableval" = xyes; then
    with_shape="yes, check"
  else
    with_shape="no"
    problem_shape=": Explicitly disabled"
  fi ],
  [ with_shape="not specified, check" ]
)

AH_TEMPLATE(SHAPE,
[   Define if you want the Shaped window extensions.
   Shaped window extensions seem to increase the window managers RSS
   by about 60 Kbytes. They provide for leaving a title-bar on the window
   without a border.
   If you don't use shaped window extension, you can either make your
   shaped windows undecorated, or live with a border and backdrop around
   all your shaped windows (oclock, xeyes)

   If you normally use a shaped window (xeyes or oclock), you might as
   well compile this extension in, since the memory cost is  minimal in
   this case (The shaped window shared libs will be loaded anyway). If you
   don't normally use a shaped window, you have to decide for yourself.

   Note: if it is compiled in, run time detection is used to make sure that
   the currently running X server supports it.])
if test ! x"$with_shape" = xno; then
  $UNSET ac_cv_lib_Xext_XShapeQueryExtension
  AC_CHECK_LIB(Xext, XShapeQueryExtension,
               with_shape=yes; AC_DEFINE(SHAPE),
               with_shape=no;
               problem_shape=": Failed to detect Shape extension",
               [$X_LIBS $X_PRE_LIBS -lX11 $X_EXTRA_LIBS])
fi

# ********* MIT Shared Memory Extension
AC_ARG_ENABLE(shm,
  AS_HELP_STRING([--disable-shm],[disable MIT Shared Memory Extension]),
  [ if test x"$enableval" = xyes; then
    with_shm="yes, check"
  else
    with_shm="no"
    problem_shm=": Explicitly disabled"
  fi ],
  [ with_shm="not specified, check" ]
)

AH_TEMPLATE([HAVE_XSHM],[Define if  MIT Shared Memory extension is used.])
if test ! x"$with_shm" = xno; then
  $UNSET ac_cv_lib_Xext_XShmQueryExtension
  AC_CHECK_LIB(Xext, XShmQueryExtension,
               with_shm=yes; AC_DEFINE(HAVE_XSHM),
               with_shm=no;
               problem_shm=": Can't detect MIT Shared Memory ext.",
               [$X_LIBS $X_PRE_LIBS -lX11 $X_EXTRA_LIBS])
fi

# Silently look for X11/XKBlib.h
AH_TEMPLATE([HAVE_X11_XKBLIB_H],[Define if Xkb extension is used.])
AC_CHECK_HEADER(X11/XKBlib.h, AC_DEFINE(HAVE_X11_XKBLIB_H))

# TAILQ and friends
AH_TEMPLATE([HAVE_TAILQ],[Check if tailq is present.])
AC_CHECK_HEADER(queue.h, AC_DEFINE(HAVE_TAILQ))

# ********* XRandR
problem_xrandr=""

AC_ARG_ENABLE(xrandr,
  AS_HELP_STRING([--disable-xrandr],[disable XRandR support]),
  [ if test x"$enableval" = xyes; then
    with_xrandr="yes, check"
  else
    with_xrandr="no"
    problem_xrandr=": Explicitly disabled"
  fi ],
  [ with_xrandr="not specified, check" ]
)

AH_TEMPLATE([HAVE_XRANDR],[Define if XRandR library is used.])
if test ! x"$with_xrandr" = xno; then
  $UNSET ac_cv_lib_XRandR_XRRQueryExtension
  AC_CHECK_LIB(Xrandr, XRRQueryExtension, [
    AC_DEFINE(HAVE_XRANDR),
    with_xrandr=yes
    XRandR_LIBS=-lXrandr
    ],[
       with_xrandr=no
       problem_xrandr="Couldn't detect libxrandr"
    ],[$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS])
fi

AC_SUBST(XRandR_LIBS)
AC_SUBST(XRandR_CFLAGS)

# ********* xrender
problem_xrender=""
AC_ARG_ENABLE(xrender,
  AS_HELP_STRING([--disable-xrender],[disable Xrender alpha-blend rendering]),
  [ if test x"$enableval" = xyes; then
    with_xrender="yes, check"
  else
    with_xrender="no"
    problem_xrender=": Explicitly disabled"
  fi ],
  [ with_xrender="not specified, check" ]
)

AH_TEMPLATE([HAVE_XRENDER],[Define if Xrender library is used.])
if test ! x"$with_xrender" = xno; then
  $UNSET ac_cv_lib_Xrender_XRenderComposite
  AC_CHECK_LIB(Xrender, XRenderComposite,
    with_xrender=yes; Xrender_LIBS=-lXrender;
    AC_DEFINE(HAVE_XRENDER),
    with_xrender=no; Xrender_LIBS=
    problem_xrender=": Failed to detect libXrender",
    [$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS])
fi
AC_SUBST(Xrender_LIBS)
AC_SUBST(Xrender_CFLAGS)

# ********* xcursor
AH_TEMPLATE([HAVE_XCURSOR],[Define if Xcursor library is used.])
xcursor_CFLAGS=""
xcursor_LIBS=""
AC_ARG_ENABLE(xcursor,
  AS_HELP_STRING([--disable-xcursor],
    [disable Xcursor ARGB/animated cursor loading]),
  [ if test x"$enableval" = xno; then
       with_xcursor=no
       problem_xcursor=": Explicitly disabled"
    fi ],
)
if test ! x"$with_xcursor" = xno; then
   with_xcursor=no
   if test ! x"$with_xrender" = xno; then
      $UNSET ac_cv_lib_Xrender_XRenderCreateCursor
      AC_CHECK_LIB(Xrender, XRenderCreateCursor, [
         with_xcursor=yes
      ],[
         problem_xcursor=": Your libXrender version is too old"
      ],
      [$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS])
   else
      problem_xcursor=": Need Xrender support"
   fi
fi
if test x"$with_xcursor" = xyes ; then
      $UNSET ac_cv_lib_Xcursor_XcursorImageLoadCursor
      AC_CHECK_LIB(Xcursor, XcursorImageLoadCursor, [
         AC_DEFINE(HAVE_XCURSOR)
         Xcursor_LIBS=-lXcursor
         problem_xcursor=""
      ],[
         with_xcursor=no
         problem_xcursor=": Failed to detect libXcursor"
      ],[$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS])
fi
AC_SUBST(Xcursor_CFLAGS)
AC_SUBST(Xcursor_LIBS)

# ********* xft
problem_xft=""

AC_ARG_ENABLE(xft,
  AS_HELP_STRING([--disable-xft],[disable Xft anti-aliased font rendering]),
  [ if test x"$enableval" = xyes; then
    with_xft="yes, check"
  else
    with_xft="no"
    problem_xft=": Explicitly disabled"
  fi ],
  [
    with_xft="not specified, check"
  ]
)

AH_TEMPLATE([HAVE_XFT],[Define if Xft library is used.])
AH_TEMPLATE([HAVE_XFT2],[Define if Xft 2 library is used.])
AH_TEMPLATE([HAVE_XFT_UTF8],[Define if Xft library can handle utf8 encoding])
if test ! x"$with_xft" = xno; then

  # first check for freetype2
  have_freetype=no
  AM_CHECK_FT2(6.1.0)
  if test x"$no_ft" = x; then
    have_freetype=yes
  else
    have_freetype=no
    problem_xft=": Can't detect freetype2 >= 6.1.0/2.0.6"
  fi

  # check for fontconfig for Xft 2
  have_fontconfig=no
  if test ! x"$have_freetype" = xno ; then
    AM_CHECK_FC(1.0.1)
    if test x"$no_fc" = x ; then
      have_fontconfig=yes
      fontconfig_CFLAGS=`$PKG_CONFIG --cflags fontconfig`
      fontconfig_LIBS=`$PKG_CONFIG --libs fontconfig`
      CFLAGS="$CFLAGS $fontconfig_CFLAGS"
      LIBS="$LIBS $fontconfig_LIBS"
    else
      have_fontconfig=no
      problem_xft=": Can't detect fontconfig >= 1.0.1"
    fi
  fi

  # now check for Xft 2
  with_xft=no
  if test ! x"$have_fontconfig" = xno ; then
    # Xft 2
    AM_CHECK_XFT(2.0.0)
    if test x"$no_xft" = x; then
      with_xft=yes
      problem_xft=" (version 2)"
      AC_DEFINE(HAVE_XFT2)
      AC_DEFINE(HAVE_XFT)
      AC_DEFINE(HAVE_XFT_UTF8)
      Xft_LIBS=$XFT_LIBS
      Xft_CFLAGS=$XFT_CFLAGS
    else
      problem_xft=": Can't detect Xft2, detected fontconfig"
    fi
  fi

  # if Xft2 not detected check for Xft1
  if test ! x"$have_freetype" = xno && test ! x"$with_xft" = xyes; then
    #Xft 1
    $UNSET ac_cv_lib_XftConfigSubstitute
    $UNSET ac_cv_lib_Xft_XftFontOpen
    $UNSET ac_cv_lib_Xft_XftDrawStringUtf8
    AC_CHECK_LIB(Xft, XftFontOpen,
      with_xft=yes, with_xft=no,
      [$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS $FT2_LIBS $Xrender_LIBS])
    if test x"$with_xft" = xyes ; then
      AC_CHECK_LIB(Xft, XftConfigSubstitute, is_xft1=yes, is_xft1=no,
        [$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS $FT2_LIBS $Xrender_LIBS])
      if test x"$is_xft1" = xyes; then
        Xft_LIBS="-lXft $FT2_LIBS"
	Xft_CFLAGS="$Xft_CFLAGS $FT2_CFLAGS"
        problem_xft=" (version 1)"
        AC_DEFINE(HAVE_XFT)
      else
        with_xft=no
        problem_xft=": Can't detect Xft 1 or fontconfig"
      fi
    else
      problem_xft=": Can't detect Xft 1 or 2 and fontconfig"
    fi
    if test x"$with_xft" = xyes; then
      AC_CHECK_LIB(Xft, XftDrawStringUtf8,
        AC_DEFINE(HAVE_XFT_UTF8),,
        [$X_LIBS $X_PRE_LIBS -lXext -lX11 $X_EXTRA_LIBS $FT2_LIBS $Xrender_LIBS])
    fi
  fi

fi
AC_SUBST(Xft_LIBS)
AC_SUBST(Xft_CFLAGS)

problem_png=": png library or header not found"

$UNSET ac_cv_header_png_h
$UNSET ac_cv_lib_png_png_read_info
smr_CHECK_LIB(png, png, for coloured or shaped icons,
    png_read_info, png.h, -lz -lm,)

if test ! x"$png_LIBS" = x; then
  # Check for proper version of png
  AC_MSG_CHECKING([for libpng 1.0.4a or better])
  my_CPPFLAGS="$CPPFLAGS"
  my_LIBS="$LIBS"
  CPPFLAGS="$CPPFLAGS $png_CFLAGS"
  LIBS="$LIBS $png_LIBS -lz -lm"
  AC_TRY_RUN([#include <png.h>
   int main(int c, char **v) {
   return c == 1 ? 0 : (PNG_LIBPNG_VER < 10005) ? 2 : 0 ;}],
    [./conftest dummy_arg; png_status=$?;
     if test x"$png_status" = x0; then
       with_png=yes;
     else
       with_png=no;
       if test x"$png_status" = x2; then
	 problem_png=": png library version is too old"
       else
	 problem_png=": Internal png detection logic error"
       fi
     fi],
   [with_png="no"; problem_png=": png test error, see config.log"],
   [echo $ac_n "cross compiling; assumed OK... $ac_c"])
  AC_MSG_RESULT($with_png)
  CPPFLAGS="$my_CPPFLAGS"
  LIBS="$my_LIBS"
fi

AH_TEMPLATE([HAVE_PNG],[Define if ppm library is used.])
if test x"$with_png" = xyes; then
  AC_DEFINE(HAVE_PNG)
  png_LIBS="$png_LIBS -lz"
  problem_png=""
else
  with_png=no
fi
AC_SUBST(png_LIBS)
AC_SUBST(png_CFLAGS)

# ** needed by the png support
AC_C_BIGENDIAN

# ********* readline
with_readline=no
problem_readline=": Both termcap and ncurses disabled"

smr_ARG_WITHLIB(termcap, , to accomplish readline)
AH_TEMPLATE([HAVE_READLINE],[Define if readline is available.])
if test ! x"$with_termcap" = xno; then
  # Forget cached values, so user can re-run configure
  $UNSET ac_cv_header_readline_history_h
  $UNSET ac_cv_lib_readline_readline
  AC_MSG_CHECKING([with termcap])
  smr_CHECK_LIB(readline, , line editing/history, readline, readline/history.h,
    $termcap_LIBS)
  if test ! x"$readline_LIBS" = x; then
    with_readline=yes
    readline_LIBS="$readline_LIBS $termcap_LIBS"
    AC_DEFINE(HAVE_READLINE)
  fi
fi
smr_ARG_WITHLIB(ncurses, , to accomplish readline)
if test ! x"$with_ncurses" = xno && test x"$readline_LIBS" = x; then
  # We couldn't use readline with termcap; try with ncurses?
  # Doesn't this seem a hacky way to do this??
  # unset cached values from last check...
  $UNSET ac_cv_header_readline_history_h
  $UNSET ac_cv_lib_readline_readline
  AC_MSG_CHECKING([with ncurses])
  dnl We can not call smr_CHECK_LIB twice here without having the usage twice.
  dnl old_LIBS=$LIBS
  dnl AC_CHECK_LIB(readline, readline, , , $ncurses_LIBS)
  dnl LIBS=$old_LIBS
  dnl if test x"$ac_cv_lib_readline_readline" = xyes; then
  dnl   AC_CHECK_HEADERS(readline/history.h)
  dnl   if test x"$ac_cv_header_readline_history_h" = xyes; then
  dnl     with_readline=yes
  dnl     readline_LIBS="$readline_LIBS $ncurses_LIBS"
  dnl     AC_DEFINE(HAVE_READLINE)
  dnl   fi
  dnl fi
  smr_CHECK_LIB(readline, , line editing/history, readline, readline/history.h,
    $ncurses_LIBS)
  if test ! x"$readline_LIBS" = x; then
    with_readline=yes
    readline_LIBS="$readline_LIBS $ncurses_LIBS"
    AC_DEFINE(HAVE_READLINE)
  fi
fi
dnl We need to check if we have GNU readline or BSD editline
AH_TEMPLATE([HAVE_GNU_READLINE],[Define if readline has full GNU interface])
if test x"$with_readline" = xyes; then
   AC_CHECK_LIB(readline, append_history, AC_DEFINE(HAVE_GNU_READLINE), [], $readline_LIBS)
fi
AC_SUBST(readline_LIBS)
AC_SUBST(readline_CFLAGS)


# Check if Xsetlocale() is available or not.
AH_TEMPLATE([X_LOCALE],[Enables to use setlocale() provided by X])
AC_CHECK_LIB(X11, _Xsetlocale, AC_DEFINE(X_LOCALE),, $X_LIBS -lX11)

# Check if Xsetlocale() is available or not.
AH_TEMPLATE([HAVE_XOUTPUT_METHOD],[Enable X output method])
AC_CHECK_LIB(X11, XOpenOM, AC_DEFINE(HAVE_XOUTPUT_METHOD),, $X_LIBS -lX11)

# ******** iconv
with_iconv=no
with_iconv_type=no
with_lib_iconv=no
with_sys_iconv=no
problem_iconv=": Explicitly disabled"

AC_ARG_ENABLE(iconv,
  AS_HELP_STRING([--disable-iconv],
    [disable support for iconv character conversion]),
  [ac_cv_iconv="$enableval"],
  [ac_cv_iconv="yes"])

AH_TEMPLATE([USE_LIBICONV],
  [define if we use libiconv (not needed in general: for example iconv is
  native with recent glibc)])
AH_TEMPLATE([HAVE_ICONV],
  [Define if iconv (in the libc) or libiconv is available])
if test ! x"$ac_cv_iconv" = xno; then
  # * first check for gnu libiconv
  $UNSET ac_cv_header_libiconv_h
  $UNSET ac_cv_lib_libiconv_libiconv_open
  smr_CHECK_LIB(
  	iconv, , if not found sys iconv is used, libiconv_open, iconv.h)
  if test ! x"$iconv_LIBS" = x; then
     AC_DEFINE(USE_LIBICONV)
     with_lib_iconv=yes
  fi
  AC_SUBST(iconv_LIBS)
  AC_SUBST(iconv_CFLAGS)

  # * check for system iconv

  AC_CHECK_FUNC(iconv_open, with_sys_iconv=yes)

  if test x"$with_lib_iconv" = xyes && test x"$with_sys_iconv" = xyes; then
     AC_MSG_WARN([***  Both system iconv and libiconv found: use libiconv])
     AC_MSG_WARN([***  Use --with-iconv-library=no to use the system iconv])
  fi

  # * libiconv found check for libcharset to get the good iconv charset
  if test x"$with_lib_iconv" = xyes; then
     CHECK_LIBCHARSET
  fi

  test x"$with_lib_iconv" = xyes -o x"$with_sys_iconv" = xyes &&
	 with_iconv=yes && problem_iconv=""
  test x"$with_lib_iconv" = xyes && with_iconv_type="yes (libiconv)"
  test x"$with_sys_iconv" = xyes && with_iconv_type="yes (from C library)"

  if test x"$with_iconv" = xno; then
    AC_MSG_WARN([*** ])
    AC_MSG_WARN([*** No iconv() implementation found in C library or libiconv])
    AC_MSG_WARN([*** Please install libiconv ftp://ftp.gnu.org/pub/gnu/libiconv/])
    AC_MSG_WARN([*** ])
  fi

  if test x"$with_iconv" = xyes; then
     AC_DEFINE(HAVE_ICONV)
     ICONV_SECOND_ARG
  fi
fi
AH_VERBATIM([_HAVE_LIBCHARSET],
[#ifdef USE_LIBICONV
/* define to use locale_charset in the place of nl_langinfog if libiconv
 * is used */
#undef HAVE_LIBCHARSET
#endif])


# ******** nl_langinfo and CODESET
AH_TEMPLATE([HAVE_CODESET],[Have nl_langinfo (CODESET)])
AC_MSG_CHECKING([for nl_langinfo (CODESET)])
AC_TRY_COMPILE([#include <langinfo.h>
#include <locale.h>],
[char *codeset = nl_langinfo(CODESET); setlocale(LC_CTYPE, "");],
   AC_DEFINE(HAVE_CODESET)
   have_codeset=yes,
   have_codeset=no)
AC_MSG_RESULT($have_codeset)


# ********* fribidi
AC_ARG_ENABLE(bidi,
  AS_HELP_STRING([--disable-bidi],[disable bi-directional text support]),
  [ if test x"$enableval" = xyes; then
    with_bidi="yes, check"
  else
    with_bidi="no"
  problem_bidi=": Explicitly disabled"
  fi ],
  [ with_bidi="not specified, check" ]
)

fribidi_min_version=0.19.2
if test ! x"$with_bidi" = xno; then
  AC_ARG_WITH(fribidi-bindir,
    AS_HELP_STRING([--with-fribidi-bindir=DIR],
      [directory of fribidi-config if not in PATH]),
    FRIBIDI_BINDIR="$withval", FRIBIDI_BINDIR=".")
  if test ! x"$PKG_CONFIG" = xno && $PKG_CONFIG --exists "fribidi >= $fribidi_min_version"; then
    FRIBIDI_CONFIG="$PKG_CONFIG fribidi"
  else
    AC_PATH_PROG(FRIBIDI_CONFIG, fribidi-config,, [$FRIBIDI_BINDIR:$PATH])
  fi

  if test x"$FRIBIDI_CONFIG" = x; then
    with_bidi=no
    problem_bidi=": pkgconfig fribidi:  fribidi version >= $fribidi_min_version needed."
  else
    Bidi_CFLAGS=`$FRIBIDI_CONFIG --cflags`
    Bidi_LIBS=`$FRIBIDI_CONFIG --libs`
    AC_CHECK_LIB(fribidi, fribidi_log2vis,
      with_bidi=yes; problem_bidi=,
      with_bidi=no; problem_bidi=": No good libs via $FRIBIDI_CONFIG",
      [$Bidi_LIBS $Bidi_CFLAGS])
  fi
fi

if test ! x"$with_bidi" = xno; then
  fribidi_in_path=yes
  ac_save_LIBS="$LIBS"
  LIBS="$LIBS $Bidi_LIBS"
  AC_TRY_RUN([#include <stdlib.h>
#include <fribidi/fribidi.h>
int main()
{
	FriBidiChar *logical_unicode_str =
		(FriBidiChar *)malloc((4 + 1) * sizeof(FriBidiChar));
	fribidi_charset_to_unicode(
		fribidi_parse_charset("iso8859-8"), "test", 4,
		logical_unicode_str);
	return 0;
}
], [:], [fribidi_in_path=no])
  LIBS="$ac_save_LIBS"
  if test ! x"$fribidi_in_path" = xyes; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $Bidi_CFLAGS"
    LIBS="$LIBS $Bidi_LIBS"
  AC_TRY_RUN([#include <stdlib.h>
#include <fribidi/fribidi.h>
int main()
{
	FriBidiChar *logical_unicode_str =
		(FriBidiChar *)malloc((4 + 1) * sizeof(FriBidiChar));
	fribidi_charset_to_unicode(
		fribidi_parse_charset("iso8859-8"), "test", 4,
		logical_unicode_str);
	return 0;
}
], [:], [with_bidi=no; problem_bidi=": Bad fribidi version, see config.log"])
    CFLAGS="$ac_save_CFLAGS"
    LIBS="$ac_save_LIBS"
  fi
  if test ! x"$fribidi_in_path" = xno; then
    Bidi_CFLAGS=
    Bidi_LIBS=-lfribidi
  fi
fi

AH_TEMPLATE([HAVE_BIDI],[Define if fribidi library is used.])
AH_TEMPLATE([FRIBIDI_CHARSET_SPELLING],
  [Support fribidi-0.10.4 and older with "CHARSET" spelling.])
if test x"$with_bidi" = xno; then
  Bidi_CFLAGS=
  Bidi_LIBS=
else
  AC_DEFINE(HAVE_BIDI)
  AC_TRY_RUN([#include <fribidi/fribidi.h>
    int main(int c, char **v) {
      return FRIBIDI_CHARSET_NOT_FOUND * 0;
    }
  ], [AC_DEFINE(FRIBIDI_CHARSET_SPELLING)])
fi
AH_VERBATIM([_FRIBIDI_CHARSET_SPELLING],
[#ifdef FRIBIDI_CHARSET_SPELLING
#  define FRIBIDI_CHAR_SET_NOT_FOUND FRIBIDI_CHARSET_NOT_FOUND
#endif])

AC_SUBST(Bidi_LIBS)
AC_SUBST(Bidi_CFLAGS)


# ********* perllib
with_perllib="yes"
problem_perllib=""
MVWM_PERLLIB=perllib
AC_ARG_ENABLE(perllib,
  AS_HELP_STRING([--disable-perllib],[disable installing mvwm perl library]),
  [ if test x"$enableval" = xno; then
    with_perllib="no"
    problem_perllib=": Explicitly disabled"
    MVWM_PERLLIB=""
  fi ],
)
AC_SUBST(MVWM_PERLLIB)

# ******* gettext
ALL_LINGUAS="ar de fr sv_SE zh_CN ru es"
MVWM_DOMAIN="mvwm"
ALL_DOMAINS="$MVWM_DOMAIN"

AC_SUBST(ALL_LINGUAS)
AC_SUBST(MVWM_DOMAIN)
AC_SUBST(MVWMTASKBAR_DOMAIN)
AC_SUBST(MVWMSCRIPT_DOMAIN)
AC_SUBST(ALL_DOMAINS)

LOCALEDIR='${datadir}'"/locale"
with_gettext="yes"
problem_gettext=""

AC_ARG_ENABLE(nls,
  AS_HELP_STRING([--disable-nls],[do not use Native Language Support]),
  [ if test x"$enableval" = xno; then
    with_gettext="no"
    problem_gettext=": Explicitly disabled"
  fi ],
)

AH_TEMPLATE([HAVE_NLS],
  [Define to 1 if translation of program messages to the user's native
  language is requested.])
if test ! x"$with_gettext" = xno; then
  AM_GNU_FGETTEXT
  if test x"$USE_NLS" = "xyes"; then
    AC_DEFINE(HAVE_NLS, 1)
  else
    with_gettext="no"
    problem_gettext=": Failed to detected GNU gettext"
  fi
else
  USE_NLS=no
fi

# libs and buil
AC_SUBST(intl_LIBS)
AC_SUBST(intl_CFLAGS)
AC_SUBST(USE_NLS)
AC_SUBST(POSUB)
AC_SUBST(LOCALEDIR)
AC_SUBST(MKINSTALLDIRS)
# programs for devel
AC_SUBST(XGETTEXT)
AC_SUBST(GMSGFMT)
AC_SUBST(MSGMERGE)
AC_SUBST(MSGFMT)
AC_SUBST(MSGUNIQ)
#catalogs
AC_SUBST(INST_LINGUAS)
AC_SUBST(CATALOGS)
AC_SUBST(POFILES)
AC_SUBST(GMOFILES)
AC_SUBST(UPDATEPOFILES)
AC_SUBST(DUMMYPOFILES)
AC_SUBST(CATOBJEXT)

# Check for REQUIRED headers and functions before going any
# further.

# The module interface uses variadic functions for message passing.
AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_FUNCS(vfprintf)
if test x$ac_cv_header_stdarg_h = xno -o \
        $ac_cv_func_vfprintf = no; then
     AC_MSG_ERROR([stdarg.h and vfprintf required])
fi

# For reaping children, mvwm needs either waitpid() or wait3()
# Some extra modules may need wait4() too
AC_CHECK_FUNCS(waitpid)
if test x$ac_cv_func_waitpid = xno; then
    AC_CHECK_FUNCS(wait3 wait4)
    if test x$ac_cv_func_wait3 = xno; then
        AC_MSG_ERROR([Either waitpid or wait3 function is required])
    fi
fi

# C89/C99 signal handling
AC_CHECK_FUNCS(sigsetjmp siglongjmp)

# Look harder for a C preprocessor for MvwmCpp
AC_PATH_PROG(MVWM_CPP, cpp, no, $PATH:/lib:/usr/lib:/usr/ccs/lib)
if test x"$MVWM_CPP" = xno; then
  MVWM_CPP=
  AC_MSG_WARN([cannot locate a C preprocessor: run MvwmCpp with -cppprog])
fi
dnl FIXME: make sure the cpp we found actually works...

# Finish checking for programs.
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
dnl lex+yacc now only needed for MvwmScript developers when syntax is changed
dnl AM_PROG_LEX
dnl AC_PROG_YACC

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(stdlib.h fcntl.h limits.h malloc.h string.h memory.h unistd.h)
AC_CHECK_HEADERS(getopt.h sys/select.h sys/systeminfo.h sys/time.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
dnl AC_STRUCT_TM
AC_TYPE_SIGNAL

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_STRFTIME
AC_FUNC_SETPGRP
AC_CHECK_FUNCS(gettimeofday mkfifo putenv setvbuf socket waitpid)
AC_CHECK_FUNCS(strdup strstr strtol memmove memcpy strchr sysconf uname div)
AC_CHECK_FUNCS(sigaction siginterrupt getpwuid)
AC_CHECK_FUNCS(setpgrp setpgid)
AC_CHECK_FUNCS(lstat)



pds_CHECK_TYPE(sig_atomic_t, int, [#include <signal.h>], [Specify a type for sig_atomic_t if it's not available.])

if test x$ac_cv_func_setvbuf = xyes; then
  AC_FUNC_SETVBUF_REVERSED
fi

AC_FUNC_SELECT
if test ! x"$ac_cv_func_select" = xyes; then
  AC_MSG_ERROR([select required])
fi

# Check for standard functions that we have replacements for.
AC_REPLACE_FUNCS(gethostname strcasecmp strncasecmp strdup strerror usleep atexit)
dnl dv: this is harmful with autoconf 2.53 and is done automatically anyway
dnl AC_SUBST(LIBOBJS)

# check for mkstemp, see the discution on this subject on the mvwm workers
# list (2001-02-16 and 2001-02-24)
AM_SAFETY_CHECK_MKSTEMP

# If we do not have atexit(), then check for on_exit()
if test x$ac_cv_func_atexit = xno; then
  AC_CHECK_FUNCS(on_exit)
fi

# Check for unsetenv function
AC_CHECK_FUNCS(unsetenv)

# Define some compatibility macros needed for config.h.
mg_DEFINE_IF_NOT([#include <X11/keysym.h>],
  [defined XK_Page_Up && defined XK_Page_Down],
  [COMPAT_OLD_KEYSYMDEF], [$X_CFLAGS],
  [Old AIX systems (3.2.5) don't define some common keysyms.])
AH_VERBATIM([_COMPAT_OLD_KEYSYMDEF],
[#ifdef COMPAT_OLD_KEYSYMDEF
#  define XK_Page_Up   XK_Prior
#  define XK_Page_Down XK_Next
#endif])

# some explicit definitions for config.h file
AH_VERBATIM([_ZEND_EXPLICIT_DEFINITIONS],
[
/**
 * The next few defines are options that are only changed from their values
 * shown here on systems that _don't_ use the configure script.
 **/

/* Enable tests for missing too many XEvents.  Usually you want this. */
#define WORRY_ABOUT_MISSED_XEVENTS 1

/* Define if the X11 ConnectionNumber is actually a file descriptor. */
#define HAVE_X11_FD 1

/* Define if fork() has unix semantics.  On VMS, no child process is created
   until after a successful exec(). */
#define FORK_CREATES_CHILD 1

/* Suffix for executable filenames; NULL if no extension needed. */
#define EXECUTABLE_EXTENSION NULL

/* Define to remove the extension from executable pathnames before calling
   exec(). */
#undef REMOVE_EXECUTABLE_EXTENSION

/* Enables the "MiniIcon" Style option to specify a small pixmap which
 * can be used as one of the title-bar buttons, shown in window list,
 * utilized by modules, etc.  Requires PIXMAP_BUTTONS to be defined
 * (see below). */
/* #undef MINI_ICONS */
/* NOTE: hard coded to 1 */
#if 1
#define FMiniIconsSupported 1
#else
#define FMiniIconsSupported 0
#endif

#if RETSIGTYPE != void
#define SIGNAL_RETURN return 0
#else
#define SIGNAL_RETURN return
#endif

/* Allow GCC extensions to work, if you have GCC. */
#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later. */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) || __STRICT_ANSI__
#    define __attribute__(x)
#  endif
/* The __-protected variants of `format' and `printf' attributes
 * are accepted by gcc versions 2.6.4 (effectively 2.7) and later. */
#  if __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 7)
#    define __format__ format
#    define __printf__ printf
#  endif
#endif

#if HAVE_ALLOCA_H
#  include <alloca.h>
#else
#  ifdef _AIX
       #pragma alloca
#  else
#    ifndef alloca /* predefined by HP cc +Olibcalls */
#       ifndef __OpenBSD__
	        char *alloca ();
#       endif
#    endif
#  endif
#endif

#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#endif
#ifdef HAVE_MEMORY_H
#  include <memory.h>
#endif
#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif

#if defined (HAVE_MALLOC_H) && !defined (__FreeBSD__) && !defined (__OpenBSD__) && !defined(__NetBSD__)
#  include <malloc.h>
#endif
#ifdef HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#ifndef HAVE_STRCHR
#  define strchr(_s,_c)   index((_s),(_c))
#  define strrchr(_s,_c)  rindex((_s),(_c))
#endif

#ifndef HAVE_MEMCPY
#  define memcpy(_d,_s,_l)  bcopy((_s),(_d),(_l))
#endif
#ifndef HAVE_MEMMOVE
#  define memmove(_d,_s,_l) bcopy((_s),(_d),(_l))
#endif

#if HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif

#ifndef HAVE_TAILQ
#	include <libs/queue.h>
#endif

#if HAVE_UNISTD_H
#  include <unistd.h>
#endif

#ifndef min
#  define min(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef max
#  define max(a,b) (((a)>(b)) ? (a) : (b))
#endif
#ifndef abs
#  define abs(a) (((a)>=0)?(a):-(a))
#endif

#include "libs/defaults.h"

#ifndef FEVENT_C
#include <X11/Xlib.h>
#include "libs/FEvent.h"
#endif

#ifndef O_NOFOLLOW
#define O_NOFOLLOW 0
#endif

#ifdef HAVE_LSTAT
#define DO_USE_LSTAT 1
#define mvwm_lstat(x,y) lstat(x,y)
#else
#define DO_USE_LSTAT 0
#define mvwm_lstat(x,y) -1
#endif

#ifdef HAVE_X11_XKBLIB_H
#include <X11/XKBlib.h>
#define mvwm_KeycodeToKeysym(d, k, l, g) \
	(XkbKeycodeToKeysym((d), (k), (g), (l)))
#else
#define mvwm_KeycodeToKeysym(d, k, x, i) (XKeycodeToKeysym((d), (k), (i)))
#endif

#ifndef HAVE_STRLCPY
#   include "libs/strlcpy.h"
#endif

])

# mainly for mvwm-config
AC_SUBST(with_bidi)
AC_SUBST(with_gtk)
AC_SUBST(with_gdkimlib)
AC_SUBST(with_gettext)
AC_SUBST(with_iconv)
AC_SUBST(with_perllib)
AC_SUBST(with_png)
AC_SUBST(with_readline)
AC_SUBST(with_shape)
AC_SUBST(with_shm)
AC_SUBST(with_sm)
AC_SUBST(with_xcursor)
AC_SUBST(with_xft)
AC_SUBST(with_xrandr)
AC_SUBST(with_xrender)
AC_SUBST(with_xpm)

dnl with autoconf <2.60 this is needed
AC_SUBST(datarootdir)
AC_SUBST(docdir)

LOCAL_BUGADDR=${MVWM_BUGADDR-${USER-${LOGNAME-`whoami`}}}
AC_SUBST(LOCAL_BUGADDR)

AC_OUTPUT(
	Makefile
	libs/Makefile
	mvwm/Makefile
	modules/Makefile
	modules/MvwmAuto/MvwmAuto.1
	modules/MvwmBacker/MvwmBacker.1
	modules/MvwmButtons/MvwmButtons.1
	modules/MvwmCommand/MvwmCommand.1
	modules/MvwmConsole/MvwmConsole.1
	modules/MvwmConsole/MvwmConsoleC.pl.1
	modules/MvwmCpp/MvwmCpp.1
	modules/MvwmEvent/MvwmEvent.1
	modules/MvwmIconMan/MvwmIconMan.1
	modules/MvwmIdent/MvwmIdent.1
	modules/MvwmPager/MvwmPager.1
	perllib/Makefile
	perllib/General/Makefile
	perllib/MVWM/Makefile
	perllib/MVWM/Module/Makefile
	perllib/MVWM/Tracker/Makefile
	perllib/MVWM/Module.pm
	modules/MvwmAuto/Makefile
	modules/MvwmBacker/Makefile
	modules/MvwmButtons/Makefile
	modules/MvwmCommand/Makefile
	modules/MvwmCommand/scripts/Makefile
	modules/MvwmConsole/Makefile
	modules/MvwmConsole/MvwmConsoleC.pl
	modules/MvwmCpp/Makefile
	modules/MvwmEvent/Makefile
	modules/MvwmIconMan/Makefile
	modules/MvwmIdent/Makefile
	modules/MvwmPager/Makefile
	modules/MvwmPerl/Makefile
	modules/MvwmPerl/MvwmPerl
)

# ---------------------------------------------------------------------------
# Original config.status is patched to make generated scripts executable.
sed 's/> $ac_file/> $ac_file; test x"`head -1 $ac_file | cut -c1-2`" = x"#!" \&\& chmod a+rx $ac_file/' <config.status >config.status-t && \
mv config.status-t config.status
chmod +x config.status
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
cat >config.status-t <<EOF
#! /bin/sh

# Original config.status is patched to satisfy processing *.in with 'make -j'.
# This bug seems to be partially fixed in autoconf-2.50 (not in all places).
UNIQUE_POSTFIX=\$\$

EOF
sed 's/conftest/conftest\${UNIQUE_POSTFIX}/g' <config.status >>config.status-t && \
mv config.status-t config.status
chmod +x config.status
# ---------------------------------------------------------------------------

eval my_bindir="`eval echo ${bindir}`"
eval my_mandir="`eval echo ${mandir}`"
eval my_moddir="`eval echo ${MVWM_MODULEDIR}`"; eval my_moddir="$my_moddir"
eval my_datdir="`eval echo ${MVWM_DATADIR}`"
eval my_docdir="`eval echo ${MVWM_DOCDIR}`"
eval my_plldir="`eval echo ${MVWM_PERLLIBDIR}`"
test x"$MVWM_PERLLIB" = x && my_plldir="(Not installed) $my_plldir"
eval my_localedir="`eval echo ${LOCALEDIR}`"
test x"$USE_NLS" = xno && my_localedir="(Not installed) $my_localdir"

echo "
Mvwm Configuration:

  Version:     $VERSION$VERSIONINFO

  Executables: $my_bindir
  Man pages:   $my_mandir
  Modules:     $my_moddir
  Data files:  $my_datdir
  Perl lib:    $my_plldir
  Locale msg:  $my_localedir $INST_LINGUAS

  With Asian bi-direct. text support? $with_bidi$problem_bidi
  With Gettext Native Lang support?   $with_gettext$problem_gettext
  With Iconv support?                 $with_iconv_type$problem_iconv
  With PNG image support?             $with_png$problem_png
  With ReadLine sup. in MvwmConsole?  $with_readline$problem_readline
  With Shaped window support?         $with_shape$problem_shape
  With Shared memory for XImage?      $with_shm$problem_shm
  With Xcursor support?               $with_xcursor$problem_xcursor
  With XRandR support?                $with_xrandr$problem_xrandr
  With Xft anti-alias font support?   $with_xft$problem_xft
  With Xrender image support?         $with_xrender$problem_xrender
"
