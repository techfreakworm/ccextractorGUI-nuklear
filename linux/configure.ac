#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([CCExtractor], [0.85], [carlos@ccextractor.org])
AC_CONFIG_AUX_DIR([build-conf])
AC_CONFIG_SRCDIR([../src/ccextractorGUI.c])
AM_INIT_AUTOMAKE([foreign subdir-objects])


# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

#Checks for "pkg-config" utility
AC_MSG_CHECKING([pkg-config m4 macros])
if test m4_ifdef([PKG_CHECK_MODULES], [yes], [no]) == yes; then
	AC_MSG_RESULT([yes]);
else
	AC_MSG_RESULT([no]);
	AC_MSG_ERROR([
pkg-config is required.])
fi

# Checks for libraries.
AC_CHECK_LIB([m], [sin], [], [AC_MSG_ERROR(Math library not installed. Install it before proceeding.)])
AC_CHECK_LIB([lept], [getLeptonicaVersion], [HAS_LEPT=1 && PKG_CHECK_MODULES([lept], [lept])], [HAS_LEPT=0])
AC_CHECK_LIB([tesseract], [TessVersion], [HAS_TESSERACT=1 && PKG_CHECK_MODULES([tesseract], [tesseract])], [HAS_TESSERACT=0])
AC_CHECK_LIB([avcodec], [avcodec_version], [HAS_AVCODEC=1 && PKG_CHECK_MODULES([libavcodec], [libavcodec])], [HAS_AVCODEC=0])
AC_CHECK_LIB([avformat], [avformat_version], [HAS_AVFORMAT=1 && PKG_CHECK_MODULES([libavformat], [libavformat])], [HAS_AVFORMAT=0])
AC_CHECK_LIB([avutil], [avutil_version], [HAS_AVUTIL=1 && PKG_CHECK_MODULES([libavutil], [libavutil])], [HAS_AVUTIL=0])
AC_CHECK_LIB([swscale], [swscale_version], [HAS_SWSCALE=1 && PKG_CHECK_MODULES([libswscale], [libswscale])], [HAS_SWSCALE=0])


#Checks and prompts if libraries found/not found to avoild failure while building
AS_IF([ test $HAS_AVCODEC -gt 0 && test $HAS_AVFORMAT -gt 0 && test $HAS_AVUTIL -gt 0 && test $HAS_SWSCALE -gt 0 && test $HAS_TESSERACT -gt 0 && test $HAS_LEPT -gt 0 ], [AC_MSG_NOTICE(Burned Subtitle Extraction: Required libraries are available)])

AS_IF([ test ! $HAS_AVCODEC -gt 0 || test ! $HAS_AVFORMAT -gt 0 || test ! $HAS_AVUTIL -gt 0 || test ! $HAS_SWSCALE -gt 0 || test ! $HAS_TESSERACT -gt 0 || test ! $HAS_LEPT -gt 0 ], [AC_MSG_NOTICE(Burned Subtitle Extraction: Required libraries not found)])

#AM_CONDITIONAL(s) for setting values to enable/disable flags in Makefile.am
AM_CONDITIONAL(HARDSUBX_IS_ENABLED, [ test $HAS_AVCODEC -gt 0 && test $HAS_AVFORMAT -gt 0 && test $HAS_AVUTIL -gt 0 && test $HAS_SWSCALE -gt 0 && test $HAS_TESSERACT -gt 0 && test $HAS_LEPT -gt 0 ])
AM_CONDITIONAL(OCR_IS_ENABLED, [ test $HAS_TESSERACT -gt 0 && test $HAS_LEPT -gt 0])
AM_CONDITIONAL(TESSERACT_PRESENT, [ test ! -z  `pkg-config --libs-only-l --silence-errors tesseract` ])
AM_CONDITIONAL(TESSERACT_PRESENT_RPI, [ test -d "/usr/include/tesseract" && test `ls -A /usr/include/tesseract | wc -l` -gt 0 ])
AM_CONDITIONAL(SYS_IS_LINUX, [ test `uname -s` = "Linux"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

