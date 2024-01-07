

all:
	echo "Run this with mingw32-make"
	cp C:/workspace/hdf5build/install_mingw/HDF5/bin/*.dll .
	gcc -g \
	-IC:/workspace/hdf5build/install_mingw/HDF5/include \
	-LC:/workspace/hdf5build/install_mingw/HDF5/bin \
	-LC:/workspace/hdf5build/install_mingw/HDF5/lib \
	-llibhdf5 \
	-llibhdf5_hl \
	demo.c -o demo.exe

clean:
	rm *.dll
	rm *.exe
 