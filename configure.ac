
AC_PREREQ([2.69])
AC_INIT([unhardlink], [0.1])
AC_CONFIG_SRCDIR([unhardlink.c])
AC_CONFIG_HEADERS([config.h])

AC_GNU_SOURCE

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_atimespec])
AC_CHECK_MEMBERS([struct stat.st_atim])

# Checks for library functions.
AC_OUTPUT
