git submodule update
cd externals/as2transition
git pull origin master
cd ../bcplusparser
git pull origin master
cd ../..
./bootstrap.sh --enable-debug && make && sudo make install
