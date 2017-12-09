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

#include <ProjectSettings.hpp>
#include <EditorSceneImporter.hpp>
#include <Ref.hpp>
#include <File.hpp>
#include <Directory.hpp>
#include <Vector3.hpp>
#include <Animation.hpp>
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

Array SimpleClass::get_extensions() const {
  Array list = Array();
  list.push_back("fbx");
  return list;
}

int64_t SimpleClass::get_import_flags() const {
  return IMPORT_SCENE | IMPORT_ANIMATION;
}

Node * SimpleClass::import_scene(const String path, const int64_t flags, const int64_t bake_fps) {
  
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
  gltfOptions.useDraco = false;
  gltfOptions.useBlendShapeNormals = false;
  gltfOptions.useBlendShapeTangents = false;

  ModelData *data_render_model = nullptr;
  RawModel raw;

  char *fbx_file = ProjectSettings::globalize_path(path).alloc_c_string();

  if(!LoadFBXFile(raw, fbx_file, godot::String("png;jpg;jpeg").alloc_c_string()))
  {
    return nullptr;
  }

  std::vector<std::function<Vec2f(Vec2f)>> texturesTransforms;
  texturesTransforms.emplace_back([](Vec2f uv) { return Vec2f(uv[0], 1.0f - uv[1]); });
 
  if (!texturesTransforms.empty()) {
    raw.TransformTextures(texturesTransforms);
  }
  raw.Condense();
 
  std::ofstream outStream; // note: auto-flushes in destructor
  const auto streamStart = outStream.tellp();

  const String dir_suffix = String("_out/");
  const String path_dir_global = ProjectSettings::globalize_path(path.get_basename().insert(path.get_basename().length(), dir_suffix));
  const String gltf_path = path_dir_global.plus_file(path.get_file() + String(".gltf")); // root/example_out/example.gltf
  godot::String gltf_global = ProjectSettings::globalize_path(gltf_path);

  godot::Ref<godot::Directory> dir = new godot::Directory;
  Error err = dir->make_dir(path_dir_global);

  outStream.open(gltf_global.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
  if (outStream.fail()) {
    Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + path);
    return nullptr;
  }

  data_render_model = Raw2Gltf(outStream, path_dir_global.alloc_c_string(), raw, gltfOptions);

  // godot::String gltf_bytes = String((unsigned long) (outStream.tellp() - streamStart));
  godot::String wrote = String("Wrote ");
  // Godot::print(
  //      String("Wrote ").inserted(wrote.length(), gltf_bytes)(" bytes of glTF to %s.\n"),
  //      , p_source_file.alloc_c_string());

  const String binary_path = ProjectSettings::globalize_path(path_dir_global.plus_file(String("buffer.bin")));
  FILE *fp = fopen(binary_path.alloc_c_string(), "wb");
  if (fp == nullptr) {
      Godot::print("ERROR:: Couldn't open file");
      //fmt::fprintf(stderr, "ERROR:: Couldn't open file '%s' for writing.\n", binaryPath);
      return nullptr;
  }

  const unsigned char *binaryData = &(*data_render_model->binary)[0];
  unsigned long       binarySize  = data_render_model->binary->size();
  if (fwrite(binaryData, binarySize, 1, fp) != 1) {
      Godot::print("ERROR: Failed to write");
      //fmt::fprintf(stderr, "ERROR: Failed to write %lu bytes to file '%s'.\n", binarySize, binaryPath);
      fclose(fp);
      return nullptr;
  }
  fclose(fp);
  Godot::print("Wrote");
  //fmt::printf("Wrote %lu bytes of binary data to %s.\n", binarySize, binaryPath);

  delete data_render_model;

  return owner->import_scene_from_other_importer(gltf_path, flags, bake_fps);
}

godot::Ref<godot::Animation> SimpleClass::import_animation(const String path, const int64_t flags, const int64_t bake_fps)
{
   return godot::Ref<Animation>();
}

void SimpleClass::_register_methods() {
  register_method("_get_extensions", &SimpleClass::get_extensions);
  register_method("_get_import_flags", &SimpleClass::get_import_flags);
  register_method("_import_scene", &SimpleClass::import_scene);
  register_method("_import_animation", &SimpleClass::import_animation);
}