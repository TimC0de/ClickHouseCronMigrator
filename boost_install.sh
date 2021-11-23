#!/bin/sh

# Download and extract boost
if [ ! -d deps/boost_1_71_0 ]
then
	wget -P deps https://boostorg.jfrog.io/artifactory/main/release/1.71.0/source/boost_1_71_0.tar.bz2
	tar -jvxf deps/boost_1_71_0.tar.bz2 -C deps
	rm deps/boost_1_71_0.tar.bz2
fi

# Bootstrap boost
cd deps/boost_1_71_0
./bootstrap.sh --prefix=/usr/local

# Executing boost installation script
./b2 install --with-program_options