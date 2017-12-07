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
  register_tool_class<ResourceImporterFBX>();
}

String ResourceImporterFBX::get_importer_name() const {
  return String("fbx_mesh");
}

String ResourceImporterFBX::get_visible_name() const {
  return String("FBX As Mesh");
}

int ResourceImporterFBX::get_preset_count() const {
  return 0;
}

String ResourceImporterFBX::get_preset_name(const int preset) const {
  return String();
}

Array ResourceImporterFBX::get_recognized_extensions() const {
  PoolStringArray recognized;
  recognized.push_back(String("fbx"));
  return recognized;
}

Array ResourceImporterFBX::get_import_options(const int preset) const {
  PoolStringArray options;
  return options;
}

String ResourceImporterFBX::get_save_extension() const {
  return String("mesh");
}

String ResourceImporterFBX::get_resource_type() const {
  return String("ArrayMesh");
}

bool ResourceImporterFBX::get_option_visibility(const String option, const Dictionary options) const {
  return true;
}

int ResourceImporterFBX::import(const String p_source_file, const String p_save_path, const Dictionary p_options, const Array p_r_platform_variants, const Array p_r_gen_files) {
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
  return GODOT_FAILED;
}

void ResourceImporterFBX::_register_methods() {
  register_method("get_importer_name", &ResourceImporterFBX::get_importer_name);
  register_method("get_visible_name", &ResourceImporterFBX::get_visible_name);
  register_method("get_preset_count", &ResourceImporterFBX::get_preset_count);
  register_method("get_preset_name", &ResourceImporterFBX::get_preset_name);
  register_method("get_recognized_extensions", &ResourceImporterFBX::get_recognized_extensions);
  register_method("get_import_options", &ResourceImporterFBX::get_import_options);
  register_method("get_save_extension", &ResourceImporterFBX::get_save_extension);
  register_method("get_resource_type", &ResourceImporterFBX::get_resource_type);
  register_method("get_option_visibility", &ResourceImporterFBX::get_option_visibility);
  register_method("import", &ResourceImporterFBX::import);
}