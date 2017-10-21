# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

```
cd "../../Editor/Godot"
scons platform=windows -j%NUMBER_OF_PROCESSORS% target=release_debug
cd "../../Editor/CppBindings"
scons godotbinpath="../Godot/bin/" headers="../GodotHeaders/" p=windows generate_bindings=yes -j%NUMBER_OF_PROCESSORS%-j%NUMBER_OF_PROCESSORS%
cd Plugins/Itchio
bazel build //:resource-importer-fbx
```
