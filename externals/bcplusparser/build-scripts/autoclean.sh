#!/bin/bash
echo "Cleaning bcplusparser..."

# Simple Script file for cleaning up autoconf files
if [ -e 'Makefile' -a -e 'config.status' ]
then
	echo "Cleaning up the build files..."
	make clean
fi 

echo "Cleaning up autoconf files..."
rm -rf aclocal.m4 autom4te.cache config.h config.h.in config.log config.status stamp-h1 configure libtool
rm -f build-scripts/depcomp  build-scripts/install-sh  build-scripts/missing  build-scripts/ylwrap 
rm -f build-scripts/config.guess build-scripts/config.sub build-scripts/ltmain.sh
find . -name 'Makefile.in' -exec rm -f {} \;
find . -name '.deps' -exec rm -rf {} \;

if [ "$1" == "all" ]
then
	echo "Cleaning up the make files..."
	find . -name 'Makefile' -exec rm -f {} \; 
fi
