@ECHO OFF

ECHO run this script from third-party folder!
TIMEOUT 5

ECHO add mingw to path
SET PATH=%PATH%;C:\opt\binw\mingw64\bin

ECHO create result folder

ECHO unzip using 7zip
"C:\Program Files\7-Zip\7z.exe" x *.zip

ECHO go to cmake
CD CMake*

ECHO create build dir
MKDIR build
CD build

ECHO run CMAKE
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release -DHDF5_BUILD_FORTRAN:BOOL=OFF -DHDF5_BUILD_JAVA:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH=..\..\HDF5 DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF  -DBUILD_TESTING:BOOL=ON -DHDF5_BUILD_TOOLS:BOOL=ON ..

ECHO make
mingw32-make -j4

ECHO install
mingw32-make install

