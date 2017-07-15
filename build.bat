REM Replace professional with community
pushd %cd%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
popd
cd /D godot_fork
scons platform=windows -j%NUMBER_OF_PROCESSORS%
cd ..
cd cpp_bindings
scons p=windows -j%NUMBER_OF_PROCESSORS%
scons p=windows target=bindings generate_bindings=yes -j%NUMBER_OF_PROCESSORS%
copy bin/* ../sample/
cd ..
scons platform=windows -j%NUMBER_OF_PROCESSORS%