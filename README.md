# Install Instructions

Open Microsoft Visual Studio native tools compiler prompt.

Type:

```
cd godot
scons platform=windows -j4
cd ..
cd cpp_bindings
scons platform=windows -j4
scons p=windows target=bindings generate_bindings=yes -j4
cd ..
scons platform=windows -j4
```
