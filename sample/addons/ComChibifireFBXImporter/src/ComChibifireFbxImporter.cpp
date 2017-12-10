// Major portions from the main source file of fbx2glTF

#include <Reference.hpp>
#include <core/Godot.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)

#include <sys/stat.h>

#define _stricmp strcasecmp
#endif

#include <cxxopts.hpp>

#include "FBX2glTF.h"
#include "Fbx2Raw.h"
#include "Raw2Gltf.h"
#include "RawModel.h"
#include "utils/File_Utils.h"
#include "utils/String_Utils.h"

#include "ComChibifireFbxImporter.h"
#include <Animation.hpp>
#include <Directory.hpp>
#include <EditorSceneImporter.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <Vector3.hpp>

bool verboseOutput = false;

using namespace godot;
using godot::Directory;
using godot::Ref;

GDNATIVE_INIT(godot_gdnative_init_options *options) {
}

GDNATIVE_TERMINATE(godot_gdnative_terminate_options *options) {
}

NATIVESCRIPT_INIT() {
    register_tool_class<ComChibifireFbxImporter>();
}

Array ComChibifireFbxImporter::get_extensions() const {
    Array list = Array();
    list.push_back("fbx");
    return list;
}

int64_t ComChibifireFbxImporter::get_import_flags() const {
    return IMPORT_SCENE | IMPORT_ANIMATION;
}

Node *ComChibifireFbxImporter::import_scene(const String path, const int64_t flags, const int64_t bake_fps) {
    GltfOptions gltfOptions{
        -1, // keepAttribs
        false, // outputBinary
        false, // embedResources
        false, // useDraco
        false, // useKHRMatCom
        false, // usePBRMetRough
        false, // usePBRSpecGloss
        false, // useBlendShapeNormals
        false, // useBlendShapeTangents
    };
    gltfOptions.usePBRMetRough = true;
    gltfOptions.useBlendShapeNormals = true;
    gltfOptions.useBlendShapeTangents = true;

    ModelData *data_render_model = nullptr;
    RawModel raw;

    const char *fbx_file = ProjectSettings::globalize_path(path).alloc_c_string();

    const String dir_suffix = String("_out/");
    const String path_dir_global = ProjectSettings::globalize_path(path.get_basename().insert(path.get_basename().length(), dir_suffix));

    Ref<Directory> dir = new Directory;
    const Error err = dir->make_dir(path_dir_global);
    if (err == ERR_ALREADY_EXISTS) {
        Godot::print(godot::String("ERROR:: Folder already exists: ") + path_dir_global);
        return nullptr;
    }

    if (!LoadFBXFile(raw, fbx_file, godot::String("png;jpg;jpeg").alloc_c_string())) {
        return nullptr;
    }

    std::vector<std::function<Vec2f(Vec2f)> > texturesTransforms;
    texturesTransforms.emplace_back([](Vec2f uv) { return Vec2f(uv[0], 1.0f - uv[1]); });

    if (!texturesTransforms.empty()) {
        raw.TransformTextures(texturesTransforms);
    }
    raw.Condense();

    std::ofstream outStream; // note: auto-flushes in destructor
    const auto streamStart = outStream.tellp();

    const String file = path.get_basename().get_file() + String(".gltf");
    const String gltf_path = path_dir_global.plus_file(file);
    const String gltf_global = ProjectSettings::globalize_path(gltf_path);

    outStream.open(gltf_global.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
    if (outStream.fail()) {
        Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + gltf_global);
        return nullptr;
    }

    data_render_model = Raw2Gltf(outStream, path_dir_global.alloc_c_string(), raw, gltfOptions);

    printf(
            "Wrote %lu bytes of glTF to %s.\n",
            (unsigned long)(outStream.tellp() - streamStart), gltf_global.alloc_c_string());

    const String binary_path = ProjectSettings::globalize_path(path_dir_global.plus_file(String("buffer.bin")));
    FILE *fp = fopen(binary_path.alloc_c_string(), "wb");
    if (fp == nullptr) {
        printf("ERROR:: Couldn't open file '%s' for writing.\n", binary_path.alloc_c_string());
        return nullptr;
    }

    const unsigned char *binaryData = &(*data_render_model->binary)[0];
    unsigned long binarySize = data_render_model->binary->size();
    if (fwrite(binaryData, binarySize, 1, fp) != 1) {
        printf("ERROR: Failed to write %lu bytes to file '%s'.\n", binarySize, binary_path.alloc_c_string());
        fclose(fp);
        return nullptr;
    }
    fclose(fp);
    printf("Wrote %lu bytes of binary data to %s.\n", binarySize, binary_path.alloc_c_string());

    delete data_render_model;

    return owner->import_scene_from_other_importer(gltf_path, flags, bake_fps);
}

Ref<Animation> ComChibifireFbxImporter::import_animation(const String path, const int64_t flags, const int64_t bake_fps) {
    return Ref<Animation>();
}

void ComChibifireFbxImporter::_register_methods() {
    register_method("_get_extensions", &ComChibifireFbxImporter::get_extensions);
    register_method("_get_import_flags", &ComChibifireFbxImporter::get_import_flags);
    register_method("_import_scene", &ComChibifireFbxImporter::import_scene);
    register_method("_import_animation", &ComChibifireFbxImporter::import_animation);
}