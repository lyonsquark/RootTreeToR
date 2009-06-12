AC_INIT([RootTreeToR], 4.1, [lyon@fnal.gov])

# ROOTSYS must be set, or else root-config doesn't work
AC_PATH_PROG([ROOTCONFIG], [root-config], [noroot], [$PATH:$ROOTSYS/bin])
 

# Find the compiler and compiler flags
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
CXX=`"${R_HOME}/bin/R" CMD config CXX`
CXXFLAGS=`"${R_HOME}/bin/R" CMD config CXXFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`
CPICFLAGS=`"${R_HOME}/bin/R" CMD config CPICFLAGS`
RINCLUDES=`"${R_HOME}/bin/R" CMD config --cppflags`

SHLIB_CXXLD=`"${R_HOME}/bin/R" CMD config SHLIB_CXXLD`
SHLIB_LDFLAGS=`"${R_HOME}/bin/R" CMD config SHLIB_LDFLAGS`
LDFLAGS=`"${R_HOME}/bin/R" CMD config LDFLAGS`
RLIBS=`"${R_HOME}/bin/R" CMD config --ldflags`


# Check that we have a good root-config
if test "${ROOTCONFIG}" = noroot; then
   AC_MSG_ERROR("Cannot find root-config. Is ROOTSYS set?")
fi

# Get the location of root stuff
root_cflags=`${ROOTCONFIG} --cflags | sed s/-Wno-long-double//`
root_glibs=`${ROOTCONFIG} --glibs`

AC_SUBST([CPPFLAGS], ["-I../inst/include ${root_cflags} ${CPPFLAGS} ${CPICFLAGS} ${RINCLUDES}"])

AC_SUBST([LDFLAGS], ["${SHLIB_CXXLD} ${SHLIB_LDFLAGS} ${LDFLAGS}"])

AC_SUBST([LIBS], ["${root_glibs} -lTreePlayer -lMinuit -lHtml -lGX11 -lGX11TTF -lCint -lCore"])

AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
