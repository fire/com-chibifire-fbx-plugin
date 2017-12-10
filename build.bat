bazel build //:SimpleClass.dll
robocopy bazel-bin sample\addons\SimpleClass\lib\ SimpleClass.dll
robocopy bazel-bin sample\addons\SimpleClass\lib\ SimpleClass.pdb