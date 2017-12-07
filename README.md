# Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install bazel -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

Install repo. See https://github.com/esrlabs/git-repo

```
mkdir com-chibifire-fbx-plugin
cd com-chibifire-fbx-plugin
repo init -u https://github.com/fire/com-chibifire-fbx-manifest.git --no-repo-verify
repo sync
```

```
build_dependencies.bat
build.bat
copy.bat
```
