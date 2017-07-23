cc_binary(
    name = "ResourceImporterFBX.dll",
    srcs = ["sample/addons/ResourceImporterFBX/src/ResourceImporterFBX.cpp"],
    copts = ["-Igodot_headers/",
    "-IFBX\ SDK/2018.1.1/include/", 
    "-Icpp_bindings/include/", 
    "-Icpp_bindings/include/core/",
    ],
    data = glob(["godot_cpp_bindings.dll", "godot_cpp_bindings.pdb", "cube.fbx", "godot_cpp_core.dll", "godot_cpp_core.pdb", "addons/**"]),
    linkshared = 1,
    linkopts = ["-Wl,godot_fork/bin/godot.windows.tools.64.lib",
    "-Wl,FBX\ SDK/2018.1.1/lib/vs2015/x64/release/libfbxsdk.lib",
    "-Wl,cpp_bindings/bin/godot_cpp_core.lib",
    "-Wl,cpp_bindings/bin/godot_cpp_bindings.lib",
    ]
)

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar", "pkg_deb")

pkg_tar(
    name = "resource-importer-fbx-bin",
    strip_prefix = "sample",
    package_dir = "addons/ResourceImporterFBX/lib",
    files = ["//:ResourceImporterFBX.dll"],
    mode = "0755",
)

pkg_tar(
    name = "resource-importer-fbx-data",
    strip_prefix = "sample",
    package_dir = "",
    files = glob(["sample/**"], exclude=["*.import", "import"]),
    mode = "0755",
)

pkg_tar(
    name = "resource-importer-fbx",
    extension = "tar.gz",
    deps = [
        ":resource-importer-fbx-bin",
        ":resource-importer-fbx-data",
    ],
)