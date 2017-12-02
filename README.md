# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

```
git clone https://github.com/fire/com-chibifire-simple-class-gdnativescript-plugin.git SimpleClass
cd SimpleClass
git clone https://github.com/GodotNativeTools/cpp_bindings.git CppBindings
git clone https://github.com/GodotNativeTools/godot_headers.git GodotHeaders
```

```
REM Update environment variables in build_dependencies.bat
build_dependencies.bat
bazel build //:SimpleClass.dll
```
