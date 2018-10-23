# Godot Fbx importer

A fbx importer for godot. The plugin converts from fbx to gltf2. 

Assumes roughness materials.

## Install Instructions

http://landinghub.visualstudio.com/visual-cpp-build-tools

Install chocolatey.

Install bazel via chocolatey.

```
chocolatey install cmake -y
choco install mingw -params "/exception:seh /threads:posix" --force
```

Install repo. See https://github.com/esrlabs/git-repo

```
mkdir com-chibifire-fbx-plugin
cd com-chibifire-fbx-plugin
repo init -u https://github.com/fire/com-chibifire-fbx-manifest.git --no-repo-verify
repo sync
```

Download fbx20181_1_fbxsdk_vs2015 from https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2018-1-1 and use 7zip to uncompress the installer to `thirdparty/fbx20181_1_fbxsdk_vs2015_win`.

```
build_dependencies.bat
# Use cmake to generate a visual studio project
# Build debug
```
