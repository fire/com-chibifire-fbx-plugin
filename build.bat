call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cd godot_fork
scons platform=windows -j%NUMBER_OF_PROCESSORS%
cd ..
cd cpp_bindings
scons p=windows -j%NUMBER_OF_PROCESSORS%
scons p=windows target=bindings generate_bindings=yes -j%NUMBER_OF_PROCESSORS%
copy bin/* ../sample/
cd ..
scons platform=windows -j%NUMBER_OF_PROCESSORS%