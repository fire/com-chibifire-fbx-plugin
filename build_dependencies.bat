set current=%cd%
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
cd /D %current%/Godot
scons p=windows target=debug -j%NUMBER_OF_PROCESSORS% use_lto=yes gdnative_wrapper=yes
cd /D %current%
Godot\bin\godot.windows.tools.64.exe --gdnative-generate-json-api CppBindings/godot_api.json
cd /D CppBindings
scons godotbinpath="../Godot/bin/" headers="../Godot/modules/gdnative/include" p=Windows generate_bindings=yes