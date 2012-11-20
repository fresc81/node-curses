echo "building with GCC - ARM";
pushd "deps/ncurses-5.7" || exit 1;
export "CFLAGS=-fPIC";
export "CXXFLAGS=-fPIC";
./configure --without-shared --with-normal --without-debug --without-ada --without-cxx-bindings --without-progs --with-pthread --enable-widec || exit 2;
make || exit 3;
popd || exit 4;
exit 0;
