# net-hdf5


## Windows build
Instructions: https://portal.hdfgroup.org/display/support/Building+HDF5+with+CMake
```
unzip CMake-hdf5-1.10.9.zip
SET PATH=C:\opt\apps\cmake-3.26.0-rc6-windows-x86_64\bin;%PATH%

cd CMake-hdf5-1.10.9
build-VS2013-32.bat

cp -R build\bin\Release ../

gcc main.c -lWs2_32 -L Release

```
ctest -S HDF5config.cmake,BUILD_GENERATOR=VS2019 -C Release -V -O hdf5.log

-G "MinGW Makefiles"
