bazel build //:com_chibifire_fbx_importer.dll
robocopy bazel-bin sample\addons\com_chibifire_fbx_importer\bin\windows.64\ com_chibifire_fbx_importer.pdb
robocopy bazel-bin sample\addons\com_chibifire_fbx_importer\bin\windows.64\ com_chibifire_fbx_importer.dll