// Major portions from the main source file of fbx2glTF 

#include <core/Godot.hpp>
#include <Reference.hpp>

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>

#if defined( __unix__ ) || defined( __APPLE__ )

#include <sys/stat.h>

#define _stricmp strcasecmp
#endif

#include <cxxopts.hpp>

#include "FBX2glTF.h"
#include "utils/String_Utils.h"
#include "utils/File_Utils.h"
#include "Fbx2Raw.h"
#include "RawModel.h"
#include "Raw2Gltf.h"

#include <platform_config.h>
#include <core/safe_refcount.h>
#include <ProjectSettings.hpp>
#include <core/list.h>
#include <EditorSceneImporter.hpp>
#include "SimpleClass.h"

bool verboseOutput = true;

using namespace godot;

GDNATIVE_INIT(godot_gdnative_init_options *options) {
}

GDNATIVE_TERMINATE(godot_gdnative_terminate_options *options) {
}

NATIVESCRIPT_INIT() {
  register_tool_class<SimpleClass>();
}

void SimpleClass::get_extensions(List<String> *r_extensions) const {
}

uint32_t SimpleClass::get_import_flags() const {
  return 0;
}

Node * SimpleClass::import_scene(const String &p_path, uint32_t p_flags, int p_bake_fps, List<String> *r_missing_deps = NULL, godot::Error *r_err = NULL) {
  if (!p_path.to_lower().ends_with(String("fbx"))) {
    return nullptr;
  }
  
//   // GltfOptions gltfOptions{
//   //   -1,            // keepAttribs
//   //   false,         // outputBinary
//   //   false,         // embedResources
//   //   false,         // useDraco
//   //   false,         // useKHRMatCom
//   //   false,         // usePBRMetRough
//   //   false,         // usePBRSpecGloss
//   //   false,         // useBlendShapeNormals
//   //   false,         // useBlendShapeTangents
//   // };
//   // gltfOptions.usePBRMetRough = true;
//   // gltfOptions.useDraco = true;
//   // gltfOptions.useBlendShapeNormals = true;
//   // gltfOptions.useBlendShapeTangents = true;
//   // ModelData *data_render_model = nullptr;
//   // RawModel raw;
//   // if(!LoadFBXFile(raw, ProjectSettings::globalize_path(p_source_file).alloc_c_string(), godot::String("png;jpg;jpeg").alloc_c_string()))
//   // {
//   //   return FAILED;
//   // }
//   // raw.Condense();
//   // std::ofstream outStream; // note: auto-flushes in destructor
//   // const auto streamStart = outStream.tellp();

//   // outStream.open(ProjectSettings::globalize_path(p_source_file).alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
//   // if (outStream.fail()) {
//   //     Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + p_source_file);
//   //      return nullptr;
//   // }
//   // data_render_model = Raw2Gltf(outStream, p_save_path.alloc_c_string(), raw, gltfOptions);

//   // // godot::String gltf_bytes = String((unsigned long) (outStream.tellp() - streamStart));
//   // // godot::String wrote = String("Wrote ");
//   // // Godot::print(
//   // //      String("Wrote ").inserted(wrote.length(), gltf_bytes)(" bytes of glTF to %s.\n"),
//   // //      , p_source_file.alloc_c_string());

//   // godot::String binaryPath = p_source_file;
//   // binaryPath = binaryPath.insert(binaryPath.length(), godot::String(extBufferFilename.c_str()));
//   // FILE *fp = fopen(binaryPath.alloc_c_string(), "wb");
//   // if (fp == nullptr) {
//   //     Godot::print("ERROR:: Couldn't open file");
//   //     //fmt::fprintf(stderr, "ERROR:: Couldn't open file '%s' for writing.\n", binaryPath);
//   //     return nullptr;
//   // }

//   // const unsigned char *binaryData = &(*data_render_model->binary)[0];
//   // unsigned long       binarySize  = data_render_model->binary->size();
//   // if (fwrite(binaryData, binarySize, 1, fp) != 1) {
//   //     Godot::print("ERROR: Failed to write");
//   //     //fmt::fprintf(stderr, "ERROR: Failed to write %lu bytes to file '%s'.\n", binarySize, binaryPath);
//   //     fclose(fp);
//   //     return FAILED;
//   // }
//   // fclose(fp);
//   // Godot::print("Wrote");
//   // //fmt::printf("Wrote %lu bytes of binary data to %s.\n", binarySize, binaryPath);

//   // delete data_render_model;
  return nullptr;
}

void SimpleClass::_register_methods() {
  register_method("_get_extensions", &SimpleClass::get_extensions);
  register_method("_get_import_flags", &SimpleClass::get_import_flags);
//  register_method("_import_scene", &SimpleClass::import_scene);
}