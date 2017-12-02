cc_binary(
    features = ["use_linker"],
    name = "SimpleClass.dll",
    srcs = ["sample/addons/SimpleClass/src/SimpleClass.cpp",
    "CppBindings/include/gdnative.hpp",
    "GodotHeaders/gdnative_api_struct.gen.h",
    "CppBindings/bin/godot_cpp_bindings.lib",
    ], 
    includes = ["Godot/modules/gdnative/include",
    "GodotHeaders/",
    "CppBindings/include/",
    "CppBindings/include/core",],  
    copts = ["-O2", "/MT"],
    data = glob(["godot_cpp_bindings.dll", "godot_cpp_bindings.pdb", "cube.fbx", "godot_cpp_core.dll", "godot_cpp_core.pdb", "addons/**"]),
    linkstatic = 1,
    linkshared = 1,
    linkopts = []
)