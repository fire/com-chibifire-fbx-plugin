# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

```
cd "godot_fork"
scons platform=windows -j%NUMBER_OF_PROCESSORS%
cd ..
cd "cpp_bindings"
scons godotbinpath="../godot_fork/bin/" headers="../godot_headers/" p=windows generate_bindings=yes -j%NUMBER_OF_PROCESSORS%
scons p=windows target=bindings
scons p=windows target=bindings generate_bindings=yes -j%NUMBER_OF_PROCESSORS%
robocopy bin ../../Game/Sample/
cd ..
bazel build //:resource-importer-fbx
```
