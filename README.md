# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

```
cd Plugins/SimpleClass
build_dependencies.bat
build.bat
bazel build //:SimpleClass.dll
```
