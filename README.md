# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
git clone https://github.com/fire/com-chibifire-simple-class-gdnativescript-plugin.git SimpleClass
cd SimpleClass
git clone https://github.com/GodotNativeTools/cpp_bindings.git CppBindings
git clone https://github.com/godotengine/godot.git" Godot
```

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

```
build_dependencies.bat
build.bat
bazel build //:SimpleClass.dll
```
