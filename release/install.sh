#!/bin/bash

DEF_BIN_PATH_ROOT="/usr/local/bin"
DEF_LIB_PATH_ROOT="/usr/local/lib"
DEF_BIN_PATH_NONROOT="$HOME/bin"
DEF_LIB_PATH_NONROOT="$HOME/lib"

LIB_PATH=""
BIN_PATH=""
IS_ROOT=0

INSTALL_F2LP="Y"
# INSTALL_ICLINGO="Y"
INSTALL_CLINGO3TO4="Y"
INSTALL_CLINGO="Y"
# INSTALL_GRINGO_3="Y"
# INSTALL_CLASP="Y"

CONTINUE="n"

if [[ $EUID -ne 0 ]]; then

	echo "This script has NOT been run as root."
	echo "Selecting user-specific install location..."
	IS_ROOT=0
	LIB_PATH=$DEF_LIB_PATH_NONROOT
	BIN_PATH=$DEF_BIN_PATH_NONROOT

else

	echo "This script has been run as root."
	echo "Selecting system-wide install location..."
	IS_ROOT=1
	LIB_PATH=$DEF_LIB_PATH_ROOT
	BIN_PATH=$DEF_BIN_PATH_ROOT
fi


# Confirm the location with the user

read -p "Select a location to install program binaries [$BIN_PATH]: " SEL
[ ! -z "$SEL" ] && BIN_PATH=$SEL;
read -p "Select a location to install program library files [$LIB_PATH]: " SEL
[ ! -z "$SEL" ] && LIB_PATH=$SEL;

while true; do
    read -p "Do you wish to install F2LP (Y/n) [$INSTALL_F2LP]? " yn
    [ -z "$yn" ] && break;
    case $yn in
        [Yy]* ) INSTALL_F2LP="Y"; break;;
        [Nn]* ) INSTALL_F2LP="n"; break;;
        * ) echo "Please answer yes (Y) or no (n).";;
    esac
done


# while true; do
#     read -p "Do you wish to install iClingo (Y/n) [$INSTALL_ICLINGO]? " yn
#     [ -z "$yn" ] && break;
#     case $yn in
#         [Yy]* ) INSTALL_ICLINGO="Y"; break;;
#         [Nn]* ) INSTALL_ICLINGO="n"; break;;
#         * ) echo "Please answer yes (Y) or no (n).";;
#     esac
# done

while true; do
    read -p "Do you wish to install Clingo3to4 (Y/n) [$INSTALL_CLINGO3TO4]? " yn
    [ -z "$yn" ] && break;
    case $yn in
        [Yy]* ) INSTALL_CLINGO3TO4="Y"; break;;
        [Nn]* ) INSTALL_CLINGO3TO4="n"; break;;
        * ) echo "Please answer yes (Y) or no (n).";;
    esac
done


while true; do
    read -p "Do you wish to install Clingo (Y/n) [$INSTALL_CLINGO]? " yn
    [ -z "$yn" ] && break;
    case $yn in
        [Yy]* ) INSTALL_CLINGO="Y"; break;;
        [Nn]* ) INSTALL_CLINGO="n"; break;;
        * ) echo "Please answer yes (Y) or no (n).";;
    esac
done

# while true; do
#     read -p "Do you wish to install Gringo 3 (Y/n) [$INSTALL_GRINGO_3]? " yn
#     [ -z "$yn" ] && break;
#     case $yn in
#         [Yy]* ) INSTALL_GRINGO_3="Y"; break;;
#         [Nn]* ) INSTALL_GRINGO_3="n"; break;;
#         * ) echo "Please answer yes (Y) or no (n).";;
#     esac
# done

# while true; do
#     read -p "Do you wish to install Clasp (Y/n) [$INSTALL_CLASP]? " yn
#     [ -z "$yn" ] && break;
#     case $yn in
#         [Yy]* ) INSTALL_CLASP="Y"; break;;
#         [Nn]* ) INSTALL_CLASP="n"; break;;
#         * ) echo "Please answer yes (Y) or no (n).";;
#     esac
# done

# Confirm Options to the user...

echo ""
echo -e "Binary install path:  \t\t$BIN_PATH"
echo -e "Library install path: \t\t$LIB_PATH"

echo ""
echo "Applications: "

echo -e "  cplus2asp\t\t\tinstall."
echo -e "  cplus2asp.bin\t\t\tinstall."
echo -e "  as2transition\t\t\tinstall."
echo -e "  clingo3to4\t\t\tinstall."


echo ""

if [ "$INSTALL_F2LP" = "Y" ]; then 
	echo -e "  f2lp\t\t\t\tinstall."
else
	echo -e "  f2lp\t\t\t\tskip."
fi

# if [ "$INSTALL_ICLINGO" = "Y" ]; then 
# 	echo -e "  iclingo\t\t\tinstall."
# else
# 	echo -e "  iclingo\t\t\tskip."
# fi

if [ "$INSTALL_CLINGO3TO4" = "Y" ]; then 
	echo -e "  clingo3to4\t\t\tinstall."
else
	echo -e "  clingo\t\t\tskip."
fi

if [ "$INSTALL_CLINGO" = "Y" ]; then 
	echo -e "  clingo\t\t\tinstall."
else
	echo -e "  clingo\t\t\tskip."
fi

# if [ "$INSTALL_GRINGO_3" = "Y" ]; then 
# 	echo -e "  gringo 3\t\t\tinstall."
# else
# 	echo -e "  gringo 3\t\t\tskip."
# fi

# if [ "$INSTALL_CLASP" = "Y" ]; then 
# 	echo -e "  clasp\t\t\t\tinstall."
# else
# 	echo -e "  clasp\t\t\t\tskip."
# fi

echo ""

while true; do
    read -p "Do you wish to continue (Y/n) [$CONTINUE]? " yn
    [ -z "$yn" ] && break;
    case $yn in
        [Yy]* ) CONTINUE="Y"; break;;
        [Nn]* ) CONTINUE="n"; break;;
        * ) echo "Please answer yes (Y) or no (n).";;
    esac
done


if [ "$CONTINUE" != "Y" ] ; then
	echo "Installation canceled. Exiting with no changes."
	exit;
fi


# The actual installation...

mkdir -p $LIB_PATH
mkdir -p $BIN_PATH

cp -r lib/cplus2asp $LIB_PATH/
cp bin/cplus2asp.bin $BIN_PATH/.
cp bin/as2transition $BIN_PATH/.
rm -f $BIN_PATH/cplus2asp > /dev/null
ln -s $LIB_PATH/cplus2asp/cplus2asp $BIN_PATH/cplus2asp 


if [ "$INSTALL_F2LP" = "Y" ]; then
	cp bin/f2lp $BIN_PATH/. 
fi

# if [ "$INSTALL_ICLINGO" = "Y" ]; then 
# 	cp bin/iclingo $BIN_PATH/. 
# fi

if [ "$INSTALL_CLINGO3TO4" = "Y" ]; then
	cp bin/clingo3to4 $BIN_PATH/. 
fi

if [ "$INSTALL_CLINGO" = "Y" ]; then 
	cp bin/clingo $BIN_PATH/. 
fi

# if [ "$INSTALL_GRINGO_3" = "Y" ]; then 
# 	cp bin/gringo $BIN_PATH/. 
# fi

# if [ "$INSTALL_CLASP" = "Y" ]; then 
# 	cp bin/clasp $BIN_PATH/. 
# fi


# Fix permissions

if [ $IS_ROOT -eq 1 ] ; then
	chown -R root:root $LIB_PATH/cplus2asp
	chmod -R a+r $LIB_PATH/cplus2asp
	chmod a+x $LIB_PATH/cplus2asp/cplus2asp
	
	chown root:root $BIN_PATH/cplus2asp.bin
	chmod a+rx $BIN_PATH/cplus2asp.bin

	chown root:root $BIN_PATH/as2transition
	chmod a+rx $BIN_PATH/as2transition


	if [ "$INSTALL_F2LP" = "Y" ]; then
		chown root:root $BIN_PATH/f2lp
		chmod a+rx $BIN_PATH/f2lp
	fi

	# if [ "$INSTALL_ICLINGO" = "Y" ]; then 
	# 	chown root:root $BIN_PATH/iclingo
	# 	chmod a+rx $BIN_PATH/iclingo
	# fi

	if [ "$INSTALL_CLINGO3TO4" = "Y" ]; then
		chown root:root $BIN_PATH/clingo3to4
		chmod a+rx $BIN_PATH/clingo3to4
	fi

	if [ "$INSTALL_CLINGO" = "Y" ]; then 
		chown root:root $BIN_PATH/clingo
		chmod a+rx $BIN_PATH/clingo
	fi

	# if [ "$INSTALL_GRINGO_3" = "Y" ]; then 
	# 	chown root:root $BIN_PATH/gringo
	# 	chmod a+rx $BIN_PATH/gringo
	# fi

	# if [ "$INSTALL_CLASP" = "Y" ]; then 
	# 	chown root:root $BIN_PATH/clasp
	# 	chmod a+rx $BIN_PATH/clasp
	# fi

else
	chmod -R ug+r $LIB_PATH/cplus2asp
	chmod ug+x $LIB_PATH/cplus2asp/cplus2asp
	chmod ug+rx $BIN_PATH/cplus2asp.bin
	chmod ug+rx $BIN_PATH/as2transition
	
	if [ "$INSTALL_F2LP" = "Y" ]; then
		chmod ug+rx $BIN_PATH/f2lp
	fi

	# if [ "$INSTALL_ICLINGO" = "Y" ]; then 
	# 	chmod ug+rx $BIN_PATH/iclingo
	# fi


	if [ "$INSTALL_CLINGO3TO4" = "Y" ]; then
		chmod ug+rx $BIN_PATH/clingo3to4
	fi

	if [ "$INSTALL_CLINGO" = "Y" ]; then 
		chmod ug+rx $BIN_PATH/clingo
	fi

	# if [ "$INSTALL_GRINGO_3" = "Y" ]; then 
	# 	chmod ug+rx $BIN_PATH/gringo
	# fi

	# if [ "$INSTALL_CLASP" = "Y" ]; then 
	# 	chmod ug+rx $BIN_PATH/clasp
	# fi
fi
