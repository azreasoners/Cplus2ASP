#! /bin/bash
SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CALL_PATH="$PWD"

# perform the bootstrap passing arguments to autoconf
cd "$SCRIPT_PATH"
if [ "$1" == "noconf" ]
then

	mkdir -p build-scripts && \
		libtoolize &&\
		aclocal -I build-scripts --install && \
		autoheader && \
		automake --add-missing && \
		autoconf
else

	mkdir -p build-scripts && \
		libtoolize &&\
		aclocal -I build-scripts --install && \
		autoheader && \
		automake --add-missing && \
		autoconf && \
		./configure "$@"
fi
cd "$CALL_PATH"

