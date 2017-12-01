REM IF GODOT BUILD IS MORE RECENT THAN JSON
REM ../../../Editor/Godot/bin/godot.windows.opt.tools.64.exe --gdnative-generate-json-api CppBindings/godot_api.json
cd CppBindings
scons godotbinpath="../../../Editor/Godot/bin/" headers="../../../Editor/Godot/modules/gdnative/include" p=Windows