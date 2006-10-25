#!/bin/sh
aclocal
\rm -rf autom4te.cache config.h config.h.in config.log config.status configure configure.in.sav depcomp stamp-h1 
touch configure.in
aclocal
autoheader
automake --add-missing
autoconf

