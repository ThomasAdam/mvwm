#!/bin/sh

# reconfigure CVS source code
# must be called from main fvwm directory
set -x
mkdir -p etc
aclocal || exit 1
autoheader || exit 2
automake --add-missing --force-missing --copy --foreign || exit 3
autoreconf || exit 4
./configure ${1+"$@"} || exit 5
