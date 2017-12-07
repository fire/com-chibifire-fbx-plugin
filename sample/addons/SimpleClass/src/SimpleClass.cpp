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

#include "SimpleClass.h"

bool verboseOutput = false;

using namespace godot;

GDNATIVE_INIT(godot_gdnative_init_options *options) {
}

GDNATIVE_TERMINATE(godot_gdnative_terminate_options *options) {
}

NATIVESCRIPT_INIT() {
  register_tool_class<SimpleClass>();
}

String SimpleClass::get_importer_name() const {
  return String("fbx_mesh");
}

String SimpleClass::get_visible_name() const {
  return String("FBX As Mesh");
}

int SimpleClass::get_preset_count() const {
  return 0;
}

String SimpleClass::get_preset_name(const int preset) const {
  return String();
}

Array SimpleClass::get_recognized_extensions() const {
  PoolStringArray recognized;
  recognized.push_back(String("fbx"));
  return recognized;
}

Array SimpleClass::get_import_options(const int preset) const {
  PoolStringArray options;
  return options;
}

String SimpleClass::get_save_extension() const {
  return String("mesh");
}

String SimpleClass::get_resource_type() const {
  return String("ArrayMesh");
}

bool SimpleClass::get_option_visibility(const String option, const Dictionary options) const {
  return true;
}

int SimpleClass::import(const String p_source_file, const godot::String p_save_path, const Dictionary p_options, const Array p_r_platform_variants, const Array p_r_gen_files) {
  GltfOptions gltfOptions{
    -1,            // keepAttribs
    false,         // outputBinary
    false,         // embedResources
    false,         // useDraco
    false,         // useKHRMatCom
    false,         // usePBRMetRough
    false,         // usePBRSpecGloss
    false,         // useBlendShapeNormals
    false,         // useBlendShapeTangents
  };
  gltfOptions.usePBRMetRough = true;
  gltfOptions.useDraco = true;
  gltfOptions.useBlendShapeNormals = true;
  gltfOptions.useBlendShapeTangents = true;
  ModelData *data_render_model = nullptr;
  RawModel raw;
  if(!LoadFBXFile(raw, p_source_file.alloc_c_string(), godot::String("png;jpg;jpeg").alloc_c_string()))
  {
    return GODOT_FAILED;
  }
  raw.Condense();
  std::ofstream outStream; // note: auto-flushes in destructor
  const auto streamStart = outStream.tellp();

  outStream.open(p_source_file.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
  if (outStream.fail()) {
      //  godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + p_source_file);
       return GODOT_FAILED;
  }
  data_render_model = Raw2Gltf(outStream, p_save_path.alloc_c_string(), raw, gltfOptions);

  // godot::printf(
  //      "Wrote %lu bytes of glTF to %s.\n",
  //      (unsigned long) (outStream.tellp() - streamStart), p_source_file.alloc_c_string());

  godot::String binaryPath = p_source_file;
  binaryPath = binaryPath.insert(binaryPath.length(), godot::String(extBufferFilename.c_str()));
  FILE *fp = fopen(binaryPath.alloc_c_string(), "wb");
  if (fp == nullptr) {
      //fmt::fprintf(stderr, "ERROR:: Couldn't open file '%s' for writing.\n", binaryPath);
      return GODOT_FAILED;
  }

  const unsigned char *binaryData = &(*data_render_model->binary)[0];
  unsigned long       binarySize  = data_render_model->binary->size();
  if (fwrite(binaryData, binarySize, 1, fp) != 1) {
      //fmt::fprintf(stderr, "ERROR: Failed to write %lu bytes to file '%s'.\n", binarySize, binaryPath);
      fclose(fp);
      return GODOT_FAILED;
  }
  fclose(fp);
  //fmt::printf("Wrote %lu bytes of binary data to %s.\n", binarySize, binaryPath);

  delete data_render_model;
  return GODOT_OK;
}

void SimpleClass::_register_methods() {
  register_method("get_importer_name", &SimpleClass::get_importer_name);
  register_method("get_visible_name", &SimpleClass::get_visible_name);
  register_method("get_preset_count", &SimpleClass::get_preset_count);
  register_method("get_preset_name", &SimpleClass::get_preset_name);
  register_method("get_recognized_extensions", &SimpleClass::get_recognized_extensions);
  register_method("get_import_options", &SimpleClass::get_import_options);
  register_method("get_save_extension", &SimpleClass::get_save_extension);
  register_method("get_resource_type", &SimpleClass::get_resource_type);
  register_method("get_option_visibility", &SimpleClass::get_option_visibility);
  register_method("import", &SimpleClass::import);
}