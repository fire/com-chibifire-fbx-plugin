// Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// -- Godot Engine <https://godotengine.org>

#include "ResourceImporterFBX.h"

#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

#include "ofbx.h"
#include <ArrayMesh.hpp>
#include <ClassDB.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Mesh.hpp>
#include <PoolArrays.hpp>
#include <Ref.hpp>
#include <Reference.hpp>
#include <ResourceSaver.hpp>
#include <SurfaceTool.hpp>
#include <Vector3.hpp>

#include <stdio.h>

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
    Ref<ArrayMesh> mesh;
    mesh = import_fbx(p_source_file, p_save_path, p_options, p_r_platform_variants, p_r_gen_files);
    if (mesh.is_null()) {
        return GODOT_FAILED;
    }
    String save_path = p_save_path;
  if (ResourceSaver::save(save_path.operator+(String(".mesh")), mesh) <= 0) {
    return GODOT_FAILED;
  }
    return GODOT_OK;
}

Ref<ArrayMesh> ResourceImporterFBX::import_fbx(const String p_source_file, const String p_save_path, const Dictionary p_options, const Array p_r_platform_variants, const Array p_r_gen_files)
{
  ofbx::IScene* g_scene = nullptr;

  FILE* fp = fopen(ProjectSettings::globalize_path(p_source_file).c_string(), "rb");
  if (!fp) return false;

  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  auto* content = new ofbx::u8[file_size];
  fread(content, 1, file_size, fp);
  g_scene = ofbx::load((ofbx::u8*)content, file_size);

  ofbx::IScene* scene = nullptr;
  ofbx::load((ofbx::u8*)content, file_size);
  int obj_idx = 0;
  int indices_offset = 0;
  int normals_offset = 0;
  int mesh_count = scene->getMeshCount();
  for (int i = 0; i < mesh_count; ++i)
  {
    //Godot::print(fp, "o obj%d\ng grp%d\n", i, obj_idx);

    const ofbx::Mesh* mesh = scene->getMesh(i);
    const ofbx::Geometry* geom = mesh->getGeometry();
    int vertex_count = geom->getVertexCount();
    const ofbx::Vec3* vertices = geom->getVertices();
    for (int i = 0; i < vertex_count; ++i)
    {
      ofbx::Vec3 v = vertices[i];
      //Godot::print((fp, "v %f %f %f\n", v.x, v.y, v.z);
    }

    bool has_normals = geom->getNormals() != nullptr;
    if (has_normals)
    {
      const ofbx::Vec3* normals = geom->getNormals();
      int count = geom->getVertexCount();

      for (int i = 0; i < count; ++i)
      {
        ofbx::Vec3 n = normals[i];
        //Godot::print((fp, "vn %f %f %f\n", n.x, n.y, n.z);
      }
    }

    bool has_uvs = geom->getUVs() != nullptr;
    if (has_uvs)
    {
      const ofbx::Vec2* uvs = geom->getUVs();
      int count = geom->getVertexCount();

      for (int i = 0; i < count; ++i)
      {
        ofbx::Vec2 uv = uvs[i];
        //Godot::print((fp, "vt %f %f\n", uv.x, uv.y);
      }
    }

    bool new_face = true;
    int count = geom->getVertexCount();
    for (int i = 0; i < count; ++i)
    {
      if (new_face)
      {
        fputs("f ", fp);
        new_face = false;
      }
      int idx = i + 1;
      int vertex_idx = indices_offset + idx;
      //Godot::print((fp, "%d", vertex_idx);

      if (has_normals)
      {
        //Godot::print((fp, "/%d", idx);
      }
      else
      {
        //Godot::print((fp, "/");
      }

      if (has_uvs)
      {
        //Godot::print((fp, "/%d", idx);
      }
      else
      {
        //Godot::print((fp, "/");
      }

      new_face = idx < 0;
      //Godot::print(new_face ? '\n' : ' ', fp);
    }

    indices_offset += vertex_count;
    ++obj_idx;
  }

  delete[] content;
  fclose(fp);
  return nullptr; // TODO Fix

/*   if (LoadScene(lSdkManager, lScene, ProjectSettings::globalize_path(p_source_file).c_string()) == false) {
    Godot::print("FBX: Can't load scene");
    return nullptr;
  }
  
    const size_t len = 128;
    char str[len];
    snprintf(str, len, "FBX node count: %d", lScene->GetNodeCount());
    Godot::print(str);

    Ref<ArrayMesh> array_mesh = new ArrayMesh;

    FbxVector4 pos, nor;
    FbxVector2 uv;

    // https://gamedev.stackexchange.com/q/93935
    FbxNode *rootNode = lScene->GetRootNode();
    if (rootNode == nullptr) {
      return nullptr;
    }

    int iChildNodeCount = rootNode->GetChildCount();

    PoolVector3Array vertices;
    PoolVector3Array normals;
    PoolVector2Array uvs;
    PoolIntArray indices;

    for (int i = 0; i < iChildNodeCount; ++i) {
      // Child Node -> Object
      FbxNode *fbxChildNode = rootNode->GetChild(i);
      FbxMesh *fbxMesh = fbxChildNode->GetMesh();
      if (!fbxMesh) {
        continue;
      }
      if (!fbxMesh->IsTriangleMesh()) {
        Godot::print("FBX: Not a triangle mesh; Skipping mesh");
        continue;
      }

      char faces[len];
      snprintf(faces, len, "FBX faces: %d", fbxMesh->GetPolygonCount());
      Godot::print(faces);

      for (int j = 0; j < fbxMesh->GetControlPointsCount(); j++) {
        int iPolySize = fbxMesh->GetPolygonSize(j);
        FbxVector4 vertex = fbxMesh->GetControlPointAt(j);

        vertices.push_back(Vector3(static_cast<real_t>(vertex[0]),
            static_cast<real_t>(vertex[2]),
            static_cast<real_t>(vertex[1])));
      }

      bool initedNormals = 0;
      if (!initedNormals) {
        fbxMesh->InitNormals();
        fbxMesh->GenerateNormals(true, false, true);
      }

      FbxArray<FbxVector4> fbx_normal;

      if (fbxMesh->GetPolygonVertexNormals(fbx_normal)) {
        for (int l = 0; l < fbx_normal.Size(); ++l) {
          normals.push_back(Vector3(static_cast<real_t>(fbx_normal[l].mData[0]), static_cast<real_t>(fbx_normal[l].mData[1]), static_cast<real_t>(fbx_normal[l].mData[2])));
        }
      }

      for (int j = 0; j < fbxMesh->GetPolygonCount(); ++j) {
        int lStartIndex = fbxMesh->GetPolygonVertexIndex(j);
        if (lStartIndex == -1) {
          return nullptr;
        }
        int *lVertices = fbxMesh->GetPolygonVertices();
        for (int i = 0; i < fbxMesh->GetPolygonSize(j); ++i) {
          indices.push_back(lVertices[lStartIndex + i]);
        }

        if (fbxMesh->GetUVLayerCount() == 0) {
          continue;
        }

        FbxLayerElementArrayTemplate<FbxVector2> *uvVertices = 0;
        if (!fbxMesh->GetTextureUV(&uvVertices)) {
          return nullptr;
        }
        for (int i = 0; i < fbxMesh->GetPolygonSize(j); ++i) {
          FbxVector2 uv = uvVertices->GetAt(fbxMesh->GetTextureUVIndex(j, i));
          uvs.push_back(Vector2(uv.mData[0], uv.mData[1]));
        }
      }
    } 

    DestroySdkObjects(lSdkManager, lScene);

    PoolVector3Array pool_vertices;
    PoolVector3Array pool_normals;
    // pool_colors;
    PoolVector2Array pool_uvs;
    PoolIntArray pool_indices;

    for (size_t i = 0; i < vertices.size(); ++i) {
      pool_vertices.push_back(vertices[i]);
      //char vertex_output[len];
      //snprintf(vertex_output, len, "Vertex: %f %f %f at index %zd", pool_vertices[i].x, pool_vertices[i].y, pool_vertices[i].z, i);
      //Godot::print(vertex_output);
    }

    for (size_t i = 0; i < normals.size(); ++i) {
      pool_normals.push_back(normals[i]);
      //char normals_output[len];
      //snprintf(normals_output, len, "Normals: %f %f %f of %zd", pool_normals[i].x, pool_normals[i].y, pool_normals[i].z, i);
      //Godot::print(normals_output);
    }

    for (size_t i = 0; i < indices.size(); ++i) {
      pool_indices.push_back(indices[i]);
      //char index_output[len];
      //snprintf(index_output, len, "Index: %d at index %zd", pool_indices[i], i);
      //Godot::print(index_output);
    }

    for (size_t i = 0; i < uvs.size(); ++i) {
      pool_uvs.push_back(uvs[i]);
      //char index_output[len];
      //snprintf(index_output, len, "Uvs: %f %f at index %zd", pool_uvs[i].x, pool_uvs[i].y, i);
      //Godot::print(index_output);
    }

    Array arrays;
    arrays.resize(ArrayType::ARRAY_MAX);

    arrays[ArrayType::ARRAY_VERTEX] = pool_vertices;
    //arrays[ArrayType::ARRAY_NORMAL] = pool_normals;
    //arrays[ArrayType::ARRAY_TEX_UV2] = pool_uvs;
    arrays[ArrayType::ARRAY_INDEX] = pool_indices;

    array_mesh->add_surface_from_arrays(PrimitiveType::PRIMITIVE_TRIANGLES, arrays);
    char faces[len];
    snprintf(faces, len, "Mesh surface count: %d", array_mesh->get_surface_count());
    Godot::print(faces);

    return array_mesh;
    */
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
