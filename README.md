# Install Instructions

As of Saturday, July 15, 2017 godot only works on vs 2015.

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
```

Install FBX 2018.1 to "FBX SDK"

Open Microsoft Visual Studio native tools compiler prompt.

Type:

```
cd "godot_fork"
scons platform=windows -j%NUMBER_OF_PROCESSORS%
cd ..
cd "cpp_bindings"
scons p=windows -j%NUMBER_OF_PROCESSORS%
scons p=windows target=bindings
scons p=windows target=bindings generate_bindings=yes -j%NUMBER_OF_PROCESSORS%
robocopy bin ../sample/
cd ..
bazel build //:resource-importer-fbx
```