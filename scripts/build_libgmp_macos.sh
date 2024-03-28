#!/bin/sh -x

gmp_dir=$1
inst=$2

echo "build_libgmp_macos.sh: gmp_dir=$gmp_dir inst=$inst"

cd $inst
mkdir build_x86
cd build_x86

echo "** Configure x86_64"
${gmp_dir}/configure --enable-cxx --prefix=${inst}/x86_64 CFLAGS="-O2 --target=x86_64-apple-darwin" CXXFLAGS="-O2 --target=x86_64-apple-darwin" LDFLAGS="-arch x86_64" --enable-fat --build=x86_64-apple-darwin --host=x86_64-apple-darwin13.0.0
echo "** Build x86_64"
make -j8 && make install

cd ..
mkdir build_arm64
cd build_arm64
echo "** Configure arm64"
${gmp_dir}/configure --enable-cxx --prefix=${inst}/arm64 CFLAGS="-O2 --target=arm64-apple-darwin" CXXFLAGS="-O2 --target=x86_64-apple-darwin" LDFLAGS="-arch arm64" --enable-fat --build=arm64-apple-darwin --host=arm64-apple-darwin13.0.0
echo "** Build arm64"
make -j8 && make install

cd ..

mkdir -p ${inst}/lib
lipo -create -output ${inst}/lib/libgmp.10.dylib ${inst}/x86_64/lib/libgmp.10.dylib ${inst}/arm64/lib/libgmp.10.dylib
lipo -create -output ${inst}/lib/libgmp.a ${inst}/x86_64/lib/libgmp.a ${inst}/arm64/lib/libgmp.a
ln -s ${inst}/lib/libgmp.10.dylib ${inst}/lib/libgmp.dylib
mv ${inst}/arm64/include ${inst}

# bash $gmp_dir/configure --prefix=${inst} CFLAGS="-arch x86_64" CPPFLAGS="-arch x86_64" LDFLAGS="-arch x86_64" ABI=64

