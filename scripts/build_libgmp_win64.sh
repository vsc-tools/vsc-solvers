#!/bin/sh

gmp_dir=$1
inst=$2

cd $inst
mkdir build
cd build

echo "** Configure "
${gmp_dir}/configure --enable-cxx --prefix=${inst} --disable-static --enable-shared

echo "** Build "
make 

g++ -shared -o gmp.dll -Wl,--whole-archive .libs/libgmp.a -Wl,--no-whole-archive

/c/Program\ Files/Microsoft\ Visual\ Studio/20*/*/VC/Tools/MSVC/*/bin/Hostx64/x64/lib /machine:x64 /def:gmp.dll

mkdir -p ${inst}/lib

cp gmp.dll gmp.exp gmp.lib ${inst}/lib
