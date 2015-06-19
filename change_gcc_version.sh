cd /usr/bin
rm cc c++ cpp gcc g++ gcc-ar gcc-nm gcc-ranlib gcov gcov-tool
ln -s gcc-$1 cc
ln -s gcc-$1 gcc
ln -s g++-$1 c++
ln -s g++-$1 g++
ln -s cpp-$1 cpp
ln -s gcc-ar-$1 gcc-ar
ln -s gcc-nm-$1 gcc-nm
ln -s gcc-ranlib-$1 gcc-ranlib
ln -s gcov-$1 gcov
ln -s gcov-tool-$1 gcov-tool
