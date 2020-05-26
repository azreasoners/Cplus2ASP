#! /bin/bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CALL_PATH="$PWD"

# NOTE: To link statically: --enable-static
# (Or was it --enable-portable? both?)

# perform the bootstrap 
echo "Bootstrapping bcplusparser..."
cd "$SCRIPT_PATH"
if [ "$1" == "noconf" ]
then

	mkdir -p build-scripts && \
		libtoolize && \
		aclocal -I build-scripts --install && \
		autoheader && \
		automake --add-missing && \
		autoconf
else

	mkdir -p build-scripts && \
		libtoolize && \
		aclocal -I build-scripts --install && \
		autoheader && \
		automake --add-missing && \
		autoconf && \
		./configure "$@"
fi
cd "$CALL_PATH"
