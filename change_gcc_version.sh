cd /usr/bin
rm cc c++ cpp gcc g++
ln -s gcc-$1 cc
ln -s gcc-$1 gcc
ln -s g++-$1 c++
ln -s g++-$1 g++
ln -s cpp-$1 cpp
