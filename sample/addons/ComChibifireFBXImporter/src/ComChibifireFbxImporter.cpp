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
#include <EditorFileSystem.hpp>
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
    PoolStringArray arr;
    arr.push_back("fbx");
    return Array(arr);
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
    gltfOptions.outputBinary = true;
    gltfOptions.usePBRMetRough = true;

    ModelData *data_render_model = nullptr;
    RawModel raw;

    const char *fbx_file = ProjectSettings::globalize_path(path).alloc_c_string();

    const String path_dir_global = ProjectSettings::globalize_path(path.get_base_dir());

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

    const String file = path.get_basename().get_file() + String(".glb");
    const String gltf_path = path_dir_global.plus_file(file);
    const String gltf_global = ProjectSettings::globalize_path(gltf_path);

    outStream.open(gltf_global.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
    if (outStream.fail()) {
        Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + gltf_global);
        return nullptr;
    }

    data_render_model = Raw2Gltf(outStream, path_dir_global.alloc_c_string(), raw, gltfOptions);

    outStream.close();

    printf(
            "Wrote %lu bytes of glTF to %s.\n",
            (unsigned long)(outStream.tellp() - streamStart), gltf_global.alloc_c_string());

    // Disable buffer.bin
    // const String binary_path = ProjectSettings::globalize_path(path_dir_global.plus_file(String("buffer.bin")));
    // const unsigned char *binary_data = &(*data_render_model->binary)[0];
    // unsigned long binary_size = data_render_model->binary->size();
    // Ref<File> binary_file = new File;
    // PoolByteArray byte_array; 
    // for(size_t i = 0; i < binary_size; ++i) {
    //     byte_array.append(binary_data[i]);
    // }
    // binary_file->open(binary_path, File::ModeFlags::WRITE);
    // binary_file->store_buffer(byte_array);
    // binary_file->close();  
    // printf("Wrote %lu bytes of binary data to %s.\n", binary_size, binary_path.alloc_c_string());

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