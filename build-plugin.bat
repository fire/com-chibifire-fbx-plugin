pushd %cd%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
popd
call scons platform=windows -j%NUMBER_OF_PROCESSORS%