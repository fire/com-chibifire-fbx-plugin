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

// Major portions from the main source file of fbx2glTF.
// See licenses.

#include <Godot.hpp>
#include <Mesh.hpp>
#include <MeshInstance.hpp>
#include <Quat.hpp>
#include <ResourceLoader.hpp>
#include <Skeleton.hpp>
#include <SurfaceTool.hpp>
#include <Texture.hpp>
#include <core/GodotGlobal.hpp>
#include <core/String.hpp>
#include <gen/Animation.hpp>
#include <gen/ArrayMesh.hpp>
#include <gen/EditorFileSystem.hpp>
#include <gen/EditorPlugin.hpp>
#include <gen/EditorSceneImporter.hpp>
#include <gen/Node.hpp>
#include <gen/Object.hpp>
#include <gen/Reference.hpp>

#include "Engine.hpp"
#include <Godot.hpp>
#include <ImageTexture.hpp>
#include <PoolArrays.hpp>
#include <ProjectSettings.hpp>
#include <Skeleton.hpp>
#include <SpatialMaterial.hpp>

#include "gltf/TextureBuilder.hpp"
#include "stb_image.h"
#include "utils/String_Utils.hpp"

#include <algorithm>

#ifndef CLAMP
#define CLAMP(m_a, m_min, m_max) (((m_a) < (m_min)) ? (m_min) : (((m_a) > (m_max)) ? m_max : m_a))
#endif

using namespace godot;
#ifndef MAX
#define MAX(m_a, m_b) (((m_a) > (m_b)) ? (m_a) : (m_b))
#endif
//struct EditorSceneImporterAssetImportInterpolate {
//
//	static real_t lerp(const real_t &a, const real_t &b, real_t c) {
//
//		return a + (b - a) * c;
//	}
//
//	static real_t catmull_rom(const real_t &p0, const real_t &p1, const real_t &p2, const real_t &p3, float t) {
//
//		float t2 = t * t;
//		float t3 = t2 * t;
//
//		return 0.5f * ((2.0f * p1) + (-p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4 * p2 - p3) * t2 + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
//	}
//
//	static real_t bezier(real_t start, real_t control_1, real_t control_2, real_t end, float t) {
//		/* Formula from Wikipedia article on Bezier curves. */
//		real_t omt = (1.0 - t);
//		real_t omt2 = omt * omt;
//		real_t omt3 = omt2 * omt;
//		real_t t2 = t * t;
//		real_t t3 = t2 * t;
//
//		return start * omt3 + control_1 * omt2 * t * 3.0 + control_2 * omt * t2 * 3.0 + end * t3;
//	}
//};
//
//struct EditorSceneImporterAssetImportInterpolateVector3 {
//
//	static Vector3 lerp(const Vector3 &a, const Vector3 &b, real_t c) {
//
//		return a + (b - a) * c;
//	}
//
//	static Vector3 catmull_rom(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float t) {
//
//		float t2 = t * t;
//		float t3 = t2 * t;
//
//		return 0.5f * ((2.0f * p1) + (-p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4 * p2 - p3) * t2 + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);
//	}
//
//	static Vector3 bezier(Vector3 start, Vector3 control_1, Vector3 control_2, Vector3 end, float t) {
//		/* Formula from Wikipedia article on Bezier curves. */
//		real_t omt = (1.0 - t);
//		real_t omt2 = omt * omt;
//		real_t omt3 = omt2 * omt;
//		real_t t2 = t * t;
//		real_t t3 = t2 * t;
//
//		return start * omt3 + control_1 * omt2 * t * 3.0 + control_2 * omt * t2 * 3.0 + end * t3;
//	}
//};
//
//struct EditorSceneImporterAssetImportInterpolateQuat {
//
//	static Quat lerp(const Quat &a, const Quat &b, float c) {
//		return a.slerp(b, c).normalized();
//	}
//
//	static Quat catmull_rom(const Quat &p0, const Quat &p1, const Quat &p2, const Quat &p3, float c) {
//		return p1.slerp(p2, c).normalized();
//	}
//
//	static Quat bezier(Quat start, Quat control_1, Quat control_2, Quat end, float t) {
//		return start.slerp(end, t).normalized();
//	}
//};
//
//Quat _interpolate_track_quat(const PoolRealArray &p_times, const Array &p_values, float p_time, AssetImportAnimation::Interpolation p_interp) {
//	//TODO RESTORE OTHER TYPES
//	//could use binary search, worth it?
//	int idx = -1;
//	for (int i = 0; i < p_times.size(); i++) {
//		if (p_times.read()[i] > p_time)
//			break;
//		idx++;
//	}
//
//	switch (p_interp) {
//		case AssetImportAnimation::INTERP_LINEAR: {
//
//			if (idx == -1) {
//				return p_values[0];
//			} else if (idx >= p_times.size() - 1) {
//				return p_values[p_times.size() - 1];
//			}
//
//			float c = (p_time - p_times.read()[idx]) / (p_times.read()[idx + 1] - p_times.read()[idx]);
//			return EditorSceneImporterAssetImportInterpolateQuat::lerp(Quat(p_values[idx]).normalized(), Quat(p_values[idx + 1]).normalized(), c);
//
//		} break;
//	}
//	return Quat(p_values[0]);
//}
//
//real_t _interpolate_track_real(const PoolRealArray &p_times, const PoolRealArray &p_values, float p_time, AssetImportAnimation::Interpolation p_interp) {
//	//TODO RESTORE OTHER TYPES
//	//could use binary search, worth it?
//	int idx = -1;
//	for (int i = 0; i < p_times.size(); i++) {
//		if (p_times.read()[i] > p_time)
//			break;
//		idx++;
//	}
//
//	switch (p_interp) {
//		case AssetImportAnimation::INTERP_LINEAR: {
//
//			if (idx == -1) {
//				return p_values.read()[0];
//			} else if (idx >= p_times.size() - 1) {
//				return p_values.read()[p_times.size() - 1];
//			}
//
//			float c = (p_time - p_times.read()[idx]) / (p_times.read()[idx + 1] - p_times.read()[idx]);
//
//			return EditorSceneImporterAssetImportInterpolate::lerp(p_values.read()[idx], p_values.read()[idx + 1], c);
//
//		} break;
//	}
//	return real_t(p_values.read()[0]);
//}
//
//Vector3 _interpolate_track_vector3(const PoolRealArray &p_times, const PoolVector3Array &p_values, float p_time, AssetImportAnimation::Interpolation p_interp) {
//	//TODO RESTORE OTHER TYPES
//	//could use binary search, worth it?
//	int idx = -1;
//	for (int i = 0; i < p_times.size(); i++) {
//		if (p_times.read()[i] > p_time)
//			break;
//		idx++;
//	}
//
//	switch (p_interp) {
//		case AssetImportAnimation::INTERP_LINEAR: {
//
//			if (idx == -1) {
//				return p_values.read()[0];
//			} else if (idx >= p_times.size() - 1) {
//				return p_values.read()[p_times.size() - 1];
//			}
//
//			float c = (p_time - p_times.read()[idx]) / (p_times.read()[idx + 1] - p_times.read()[idx]);
//
//			return EditorSceneImporterAssetImportInterpolateVector3::lerp(p_values.read()[idx], p_values.read()[idx + 1], c);
//
//		} break;
//	}
//	return Vector3(p_values.read()[0]);
//}

// clang-format off
#include <Reference.hpp>
#include <Godot.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "FBX2glTF.h"
#include "fbx/Fbx2Raw.hpp"
#include "gltf/Raw2Gltf.hpp"
#include "raw/RawModel.hpp"

#include "ComChibifireFbxImporter.h"
#include <Animation.hpp>
#include <Directory.hpp>
#include <EditorSceneImporter.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <EditorFileSystem.hpp>
#include <Vector3.hpp>
#include <Engine.hpp>
// clang-format on

bool verboseOutput = false;

using namespace godot;
using godot::Directory;
using godot::Ref;

Array ComChibifireFbxImporter::get_extensions() const {
	PoolStringArray arr;
	arr.push_back("fbx");
	return Array(arr);
}

int64_t ComChibifireFbxImporter::get_import_flags() const {
	return IMPORT_SCENE | IMPORT_ANIMATION;
}

Node *ComChibifireFbxImporter::_import_scene(const String path, const int64_t flags, const int64_t bake_fps) {
	//GltfOptions gltfOptions;
	//gltfOptions.keepAttribs = RAW_VERTEX_ATTRIBUTE_AUTO;
	//gltfOptions.outputBinary = true;
	//gltfOptions.embedResources = true;
	//gltfOptions.draco.enabled = false;
	//gltfOptions.draco.compressionLevel = -1;
	//gltfOptions.draco.quantBitsPosition = -1;
	//gltfOptions.draco.quantBitsTexCoord = -1;
	//gltfOptions.draco.quantBitsNormal = -1;
	//gltfOptions.draco.quantBitsColor = -1;
	//gltfOptions.draco.quantBitsGeneric = -1;
	//gltfOptions.useKHRMatUnlit = false;
	//gltfOptions.usePBRMetRough = true;
	//gltfOptions.useBlendShapeNormals = false;
	//gltfOptions.useBlendShapeTangents = false;
	//gltfOptions.computeNormals = ComputeNormalsOption::MISSING;
	// compute vertex normals from geometry.
	//gltfOptions.useLongIndices = UseLongIndicesOptions::AUTO; // When to use 32-bit indices.
	RawModel raw;

	const String fbx_file = Engine::get_singleton()->get_singleton("ProjectSettings")->call("globalize_path", path);
	const String path_dir_global = Engine::get_singleton()->get_singleton("ProjectSettings")->call("globalize_path", path.get_base_dir());

	if (!LoadFBXFile(raw, fbx_file.alloc_c_string(), godot::String("png;jpg;jpeg").alloc_c_string())) {
		return nullptr;
	}

	std::vector<std::function<Vec2f(Vec2f)> > texturesTransforms;
	texturesTransforms.emplace_back([](Vec2f uv) { return Vec2f(uv[0], 1.0f - uv[1]); });

	if (!texturesTransforms.empty()) {
		raw.TransformTextures(texturesTransforms);
	}

	raw.Condense();
	raw.TransformGeometry(ComputeNormalsOption::MISSING);

	ImportState state = { &raw, path, Array() };
	Spatial *root = Spatial::_new();

	Array skeletons;
	Array bone_names;

	//Map<String, Transform>
	Dictionary bind_xforms; //temporary map to store bind transforms
	//guess the skeletons, since assimp does not really support them directly
	//Map<String, int>
	Dictionary ownership; //bone names to groups
	RawNode raw_root_node = raw.GetNode(raw.GetNodeById(raw.GetRootNode()));
	//fill this map with bone names and which group where they detected to, going mesh by mesh
	_generate_bone_groups(state, raw_root_node, ownership, bind_xforms);
	////Map<int, int>
	Dictionary skeleton_map; //maps previously created groups to actual skeletons
	//generates the skeletons when bones are found in the hierarchy, and follows them (including gaps/holes).
	_generate_skeletons(state, raw_root_node, ownership, skeleton_map, bind_xforms);
	//TODO(Ernest) Draco compression
	std::vector<RawModel> material_models;
	raw.CreateMaterialModels(
			material_models,
			false,
			-1,
			true);

	_generate_node(state, raw_root_node, root, root, bone_names);
	Dictionary mesh_cache;
	for (const auto &surface_model : material_models) {
		assert(surface_model.GetSurfaceCount() == 1);
		for (size_t i = 0; i < surface_model.GetSurfaceCount(); i++) {
			String name = _convert_name(surface_model.GetSurface(0).name);
			Godot::print("FBX instancing mesh: " + name);

			Ref<ArrayMesh> arr_mesh;
			arr_mesh.instance();
			MeshInstance *mi = NULL;
			Node *node = root->find_node(name);
			if (!node) {
				continue;
			}
			mi = Object::cast_to<MeshInstance>(node);
			if (!mi) {
				continue;
			}
			if (mesh_cache.has(name)) {
				Ref<ArrayMesh> arr = mesh_cache[name];
				mi->set_mesh(arr->duplicate(true));
				continue;
			}

			Array arrays;
			arrays.resize(ArrayMesh::ARRAY_MAX);

			PoolVector3Array normals = PoolVector3Array();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_NORMAL) != 0) {
				const AttributeDefinition<Vec3f> ATTR_NORMAL(
						"NORMAL",
						&RawVertex::normal,
						GLT_VEC3F,
						draco::GeometryAttribute::NORMAL,
						draco::DT_FLOAT32);
				std::vector<Vec3f> attribArrUV0;
				surface_model.GetAttributeArray<Vec3f>(attribArrUV0, ATTR_NORMAL.rawAttributeIx);

				for (auto a : attribArrUV0) {
					normals.push_back(Vector3(a.x, a.y, a.z));
				}
				arrays[ArrayMesh::ARRAY_NORMAL] = normals;
			}

			PoolColorArray colors = PoolColorArray();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_COLOR) != 0) {
				const AttributeDefinition<Vec4f> ATTR_COLORS(
						"COLOR",
						&RawVertex::color,
						GLT_VEC4F,
						draco::GeometryAttribute::COLOR,
						draco::DT_FLOAT32);
				std::vector<Vec4f> attribArrColor;
				surface_model.GetAttributeArray<Vec4f>(attribArrColor, ATTR_COLORS.rawAttributeIx);

				for (auto a : attribArrColor) {
					colors.push_back(Color(a.x, a.y, a.z, a.w));
				}
				arrays[ArrayMesh::ARRAY_COLOR] = colors;
			}

			PoolVector2Array uv0s = PoolVector2Array();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_UV0) != 0) {
				const AttributeDefinition<Vec2f> ATTR_TEXCOORD_0(
						"TEXCOORD_0",
						&RawVertex::uv0,
						GLT_VEC2F,
						draco::GeometryAttribute::TEX_COORD,
						draco::DT_FLOAT32);
				std::vector<Vec2f> attribArrUV0;
				surface_model.GetAttributeArray<Vec2f>(attribArrUV0, ATTR_TEXCOORD_0.rawAttributeIx);

				for (auto a : attribArrUV0) {
					uv0s.push_back(Vector2(a.x, a.y));
				}
				arrays[ArrayMesh::ARRAY_TEX_UV] = uv0s;
			}
			PoolVector2Array uv1s = PoolVector2Array();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_UV1) != 0) {
				const AttributeDefinition<Vec2f> ATTR_TEXCOORD_1(
						"TEXCOORD_1",
						&RawVertex::uv1,
						GLT_VEC2F,
						draco::GeometryAttribute::TEX_COORD,
						draco::DT_FLOAT32);
				std::vector<Vec2f> attribArrUV1;
				surface_model.GetAttributeArray<Vec2f>(attribArrUV1, ATTR_TEXCOORD_1.rawAttributeIx);

				for (auto a : attribArrUV1) {
					uv1s.push_back(Vector2(a.x, a.y));
				}
				arrays[ArrayMesh::ARRAY_TEX_UV2] = uv1s;
			}
			PoolVector3Array vertices = PoolVector3Array();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_POSITION) != 0) {
				const AttributeDefinition<Vec3f> ATTR_POSITION(
						"POSITION",
						&RawVertex::position,
						GLT_VEC3F,
						draco::GeometryAttribute::POSITION,
						draco::DT_FLOAT32);
				std::vector<Vec3f> attribArr;
				surface_model.GetAttributeArray<Vec3f>(attribArr, ATTR_POSITION.rawAttributeIx);

				for (auto a : attribArr) {
					vertices.push_back(Vector3(a.x, a.y, a.z));
				}
				arrays[ArrayMesh::ARRAY_VERTEX] = vertices;
			}
			PoolRealArray bone_indices = PoolRealArray();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_JOINT_INDICES) != 0) {
				const AttributeDefinition<Vec4i> ATTR_JOINTS(
						"JOINTS_0",
						&RawVertex::jointIndices,
						GLT_VEC3F,
						draco::GeometryAttribute::GENERIC,
						draco::DT_UINT16);
				std::vector<Vec4i> attribArr;
				surface_model.GetAttributeArray<Vec4i>(attribArr, ATTR_JOINTS.rawAttributeIx);

				for (auto a : attribArr) {
					//RawNode raw_node = state.scene->GetNode(rawSurface.jointIds[a.x]);
					real_t bone_idx = 0; //state.skeleton->find_bone(_convert_name(raw_node.name));
					bone_indices.push_back(bone_idx);
					//raw_node = state.scene->GetNode(rawSurface.jointIds[a.y]);
					//bone_idx = state.skeleton->find_bone(_convert_name(raw_node.name));
					bone_indices.push_back(bone_idx);
					//raw_node = state.scene->GetNode(rawSurface.jointIds[a.z]);
					//bone_idx = state.skeleton->find_bone(_convert_name(raw_node.name));
					bone_indices.push_back(bone_idx);
					//raw_node = state.scene->GetNode(rawSurface.jointIds[a.w]);
					//bone_idx = state.skeleton->find_bone(_convert_name(raw_node.name));
					bone_indices.push_back(bone_idx);
				}
				arrays[ArrayMesh::ARRAY_BONES] = bone_indices;
			}
			PoolRealArray bone_weights = PoolRealArray();
			if ((surface_model.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_JOINT_WEIGHTS) != 0) {
				const AttributeDefinition<Vec4f> ATTR_WEIGHTS(
						"WEIGHTS_0",
						&RawVertex::jointWeights,
						GLT_VEC3F,
						draco::GeometryAttribute::GENERIC,
						draco::DT_UINT16);
				std::vector<Vec4f> attribArr;
				surface_model.GetAttributeArray<Vec4f>(attribArr, ATTR_WEIGHTS.rawAttributeIx);

				for (auto a : attribArr) {
					bone_weights.push_back(0.0f);
					bone_weights.push_back(0.0f);
					bone_weights.push_back(0.0f);
					bone_weights.push_back(0.0f);
				}
				arrays[ArrayMesh::ARRAY_WEIGHTS] = bone_weights;
			}
			PoolIntArray idxs = PoolIntArray();
			for (int i = 0; i < surface_model.GetTriangleCount(); i++) {
				idxs.push_back(surface_model.GetTriangle(i).verts[2]);
				idxs.push_back(surface_model.GetTriangle(i).verts[1]);
				idxs.push_back(surface_model.GetTriangle(i).verts[0]);
			}
			arrays[ArrayMesh::ARRAY_INDEX] = idxs;
			int32_t idx = arr_mesh->get_surface_count();
			arr_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
			arr_mesh->surface_set_name(idx, name);

			// TODO(Ernest) Cache code
			Ref<Material> material = _generate_material_from_index(state, surface_model.GetMaterial(0), surface_model.GetMaterial(0).type);
			arr_mesh->surface_set_material(idx, material);
			if (mi) {
				mi->set_mesh(arr_mesh);
			}
			mesh_cache[name] = arr_mesh;
		}
	}
	return root;
}

godot::Ref<godot::Animation> ComChibifireFbxImporter::_import_animation(const String path, const int64_t flags, const int64_t bake_fps) {
	return Ref<Animation>();
}

void ComChibifireFbxImporter::_register_methods() {
	register_method("_get_extensions", &ComChibifireFbxImporter::get_extensions);
	register_method("_get_import_flags", &ComChibifireFbxImporter::get_import_flags);
	register_method("_import_scene", &ComChibifireFbxImporter::_import_scene);
	register_method("_import_animation", &ComChibifireFbxImporter::_import_animation);
}

void ComChibifireFbxImporter::_generate_bone_groups(const ImportState &p_state, const RawNode &p_node, Dictionary &p_ownership, Dictionary p_bind_xforms) {
	Transform mesh_offset = _get_transform(p_node.rotation, p_node.scale, p_node.translation);

	for (uint32_t l = 0; l < p_state.scene->GetSurfaceCount(); l++) {
		const RawSurface &surface = p_state.scene->GetSurface(l);
		int owned_by = -1;
		for (uint32_t j = 0; j < surface.jointIds.size(); j++) {
			const int64_t bone_id = surface.jointIds[j];
			if (p_ownership.has(bone_id)) {
				owned_by = p_ownership[bone_id];
				break;
			}
		}

		if (owned_by == -1) { //not owned, create new unique id
			owned_by = 1;
			for (size_t i = 0; i < p_ownership.size(); i++) {
				owned_by = MAX(int64_t(p_ownership[i]) + 1, owned_by);
			}
		}

		for (uint32_t j = 0; j < surface.jointIds.size(); j++) {
			const int64_t bone_id = surface.jointIds[j];
			p_ownership[bone_id] = owned_by;
			//store the full path for the bone transform
			//when skeleton finds its place in the tree, it will be restored
			Mat4f m = surface.inverseBindMatrices[l];
			Transform xform;
			xform.set(m.GetColumn(0).x, m.GetColumn(0).y, m.GetColumn(0).z, m.GetColumn(1).x, m.GetColumn(1).y, m.GetColumn(1).z, m.GetColumn(2).x, m.GetColumn(2).y, m.GetColumn(2).z, m.GetColumn(3).x, m.GetColumn(3).y, m.GetColumn(3).z);
			p_bind_xforms[bone_id] = mesh_offset * xform;
		}
	}

	for (size_t k = 0; k < p_node.childIds.size(); k++) {
		_generate_bone_groups(p_state, p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[k])), p_ownership, p_bind_xforms);
	}
}

void ComChibifireFbxImporter::_generate_skeletons(ImportState &p_state, const RawNode &p_node, Dictionary ownership, Dictionary skeleton_map, Dictionary bind_xforms) {

	//find skeletons at this level, there may be multiple root nodes for each
	//Map<int, List<aiNode *> >
	Dictionary skeletons_found;
	for (size_t i = 0; i < p_node.childIds.size(); i++) {
		int64_t id = p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[i])).id;
		if (ownership.has(id)) {
			int skeleton = ownership[id];
			if (!skeletons_found.has(skeleton)) {
				skeletons_found[skeleton] = Array();
			}
			Array(skeletons_found[skeleton]).push_back(p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[i])).id);
		}
	}

	//go via the potential skeletons found and generate the actual skeleton
	//for (Map<int, List<aiNode *> >::Element *E = skeletons_found.front(); E; E = E->next()) {

	for (size_t i = 0; i < skeletons_found.size(); i++) {
		int64_t id = skeletons_found.keys()[i];
		if (skeleton_map.has(id)) {
			//skeleton already exists? this can't be.. skip
			continue;
		}
		Skeleton *skeleton = Skeleton::_new();
		//this the only way to reliably use multiple meshes with one skeleton, at the cost of less precision
		//skeleton->set_use_bones_in_world_transform(true);
		skeleton_map[skeletons_found.keys()[i]] = p_state.skeletons.size();
		p_state.skeletons.push_back(skeleton);
		int holecount = 1;
		//fill the bones and their relationships
		// TODO(Ernest) Fix
		Array arr = skeletons_found[id];
		for (size_t j = 0; j < arr.size(); j++) {
			_fill_node_relationships(p_state, arr[j], ownership, skeleton_map, skeletons_found.keys()[i], skeleton, "", holecount, Array(), bind_xforms);
		}
	}
	//go to the children
	for (uint32_t i = 0; i < p_node.childIds.size(); i++) {
		int64_t id = p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[i])).id;
		if (ownership.has(id)) {
			continue; //a bone, so don't bother with this
		}
		_generate_skeletons(p_state, p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[i])), ownership, skeleton_map, bind_xforms);
	}
}

void ComChibifireFbxImporter::_generate_node(const ImportState &p_state, const RawNode &p_node, Node *p_parent, Node *p_owner, Array &r_bone_name) {
	String node_name = _convert_name(p_node.name);
	Transform xform = _get_transform(p_node.rotation, p_node.scale, p_node.translation);
	Node *node = NULL;
	if (p_node.surfaceId != 0) {
		MeshInstance *mi = MeshInstance::_new();
		//Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(p_state.skeletons));
		//mi->set_skeleton_path(mi->get_path_to(state.skeleton));
		node = mi;
	} else if (p_state.bone_owners.has(p_node.id)) {

		//have to actually put the skeleton somewhere, you know.
		Skeleton *skeleton = Object::cast_to<Skeleton>(Object::___get_from_variant(p_state.skeletons[p_state.bone_owners[p_node.id]]));
		if (skeleton->get_parent()) {
			//a bone for a skeleton already added..
			//could go downwards here to add meshes children of skeleton bones
			//but let's not support it for now.
			return;
		}
		//restore rest poses to local, now that we know where the skeleton finally is
		Transform skeleton_transform;
		if (p_node.parentId != p_state.scene->GetRootNode()) {
			skeleton_transform = _get_global_node_transform(p_state, p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.parentId)));
		}
		for (int i = 0; i < skeleton->get_bone_count(); i++) {
			Transform rest = skeleton_transform.affine_inverse() * skeleton->get_bone_rest(i);
			skeleton->set_bone_rest(i, rest.affine_inverse());
		}

		//skeleton->localize_rests();
		node_name = "Skeleton"; //don't use the bone root name
		xform = Transform(); //dont transform

		node = skeleton;
	} else {
		Spatial *spatial = Spatial::_new();
		node = spatial;
	}

	{
		node->set_name(node_name);
		Object::cast_to<Spatial>(node)->set_transform(xform);
		p_parent->add_child(node);
		node->set_owner(p_owner);
	}

	for (size_t i = 0; i < p_node.childIds.size(); i++) {
		_generate_node(p_state, p_state.scene->GetNode(p_state.scene->GetNodeById(p_node.childIds[i])), node, p_owner, r_bone_name);
	}
}

String ComChibifireFbxImporter::_convert_name(const std::string str) {
	String s = str.c_str();
	s = s.replace(".", "");
	if (s.split(":").size() > 1) {
		s = s.split(":")[1];
	}
	return s;
}

godot::Transform ComChibifireFbxImporter::_get_transform(Quatf rotation, Vec3f scale, Vec3f translation) {
	Transform xform;
	Mat3f m = rotation.ToMatrix().Transpose();
	xform.basis.set(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
	xform.basis.scale(Vector3(scale.x, scale.y, scale.z));
	xform.origin = Vector3(translation.x, translation.y, translation.z);
	return xform;
}

godot::Transform ComChibifireFbxImporter::_get_global_node_transform(const ImportState &p_state, const RawNode &p_node) {
	int64_t current_node = p_node.id;
	Transform xform;
	while (current_node != p_state.scene->GetRootNode()) {
		const RawNode &node = p_state.scene->GetNode(p_state.scene->GetNodeById(current_node));
		xform = _get_transform(node.rotation, node.scale, node.translation) * xform;
		current_node = node.parentId;
	}
	return xform;
}

void ComChibifireFbxImporter::_fill_node_relationships(ImportState &p_state, int64_t node_idx, Dictionary ownership, Dictionary skeleton_map, int p_skeleton_id, godot::Skeleton *p_skeleton, String parent_name, int holecount, godot::Array p_holes, Dictionary bind_xforms) {
	int64_t id = node_idx;
	//if (id == -1) {
	//	id = "AuxiliaryBone" + itos(holecount++);
	//}
	const RawNode &raw_node = p_state.scene->GetNode(p_state.scene->GetNodeById(id));
	Transform pose = _get_transform(raw_node.rotation, raw_node.scale, raw_node.translation);

	if (!ownership.has(id)) {
		//not a bone, it's a hole
		Array holes = p_holes;
		Dictionary hole;
		hole["id"] = id;
		hole["parent_name"] = parent_name;
		hole["pose"] = pose;
		hole["node"] = raw_node.id;
		holes.push_back(hole);

		for (size_t i = 0; i < raw_node.childIds.size(); i++) {
			_fill_node_relationships(p_state, raw_node.childIds[i], ownership, skeleton_map, p_skeleton_id, p_skeleton, id, holecount, holes, bind_xforms);
		}

		return;
	} else if (int64_t(ownership[id]) != p_skeleton_id) {
		//oh, it's from another skeleton? fine.. reparent all bones to this skeleton.
		int prev_owner = ownership[id];
		if (skeleton_map.has(prev_owner)) {
			Godot::print(String("A previous skeleton exists for bone '") + String(id) + String("', this type of skeleton layout is unsupported."));
			return;
		}
		for (size_t i = 0; i < ownership.size(); i++) {
			if (int64_t(ownership[ownership.keys()[i]]) == prev_owner) {
				ownership[ownership.keys()[i]] = p_skeleton_id;
			}
		}
	}

	//valid bone, first fill holes if needed
	for (int i = 0; i < p_holes.size(); i++) {

		int bone_idx = p_skeleton->get_bone_count();

		const RawNode &node = p_state.scene->GetNode(p_state.scene->GetNodeById(p_holes[i]["id"]));
		p_skeleton->add_bone(_convert_name(node.name));
		String parent_name = _convert_name(p_state.scene->GetNode(p_state.scene->GetNodeById(node.parentId)).name);
		int parent_idx = p_skeleton->find_bone(parent_name);
		if (parent_idx >= 0) {
			p_skeleton->set_bone_parent(bone_idx, parent_idx);
		}
		Transform pose_transform = _get_transform(node.rotation, node.scale, node.translation);
		p_skeleton->set_bone_rest(bone_idx, pose_transform);

		p_state.bone_owners[p_holes[i]["id"]] = skeleton_map[p_skeleton_id];
	}

	//finally fill bone

	int bone_idx = p_skeleton->get_bone_count();
	const RawNode &node = p_state.scene->GetNode(p_state.scene->GetNodeById(id));
	String node_name = _convert_name(node.name);
	p_skeleton->add_bone(node_name);
	int parent_idx = p_skeleton->find_bone(parent_name);
	if (parent_idx >= 0) {
		p_skeleton->set_bone_parent(bone_idx, parent_idx);
	}
	//p_skeleton->set_bone_pose(bone_idx, pose);
	if (bind_xforms.has(id)) {
		//for now this is the full path to the bone in rest pose
		//when skeleton finds it's place in the tree, it will get fixed
		p_skeleton->set_bone_rest(bone_idx, bind_xforms[id]);
	}
	p_state.bone_owners[id] = skeleton_map[p_skeleton_id];
	//go to children
	for (size_t i = 0; i < raw_node.childIds.size(); i++) {
		_fill_node_relationships(p_state, raw_node.childIds[i], ownership, skeleton_map, p_skeleton_id, p_skeleton, node_name, holecount, Array(), bind_xforms);
	}
}

void ComChibifireFbxImporter::_find_texture_path(const String &r_p_path, String &r_path, bool &r_found) {
	Directory *dir = Directory::_new();

	Array exts;
	exts.push_back(String("jpg"));
	exts.push_back(String("png"));
	exts.push_back(String("jpeg"));
	exts.push_back(String("dds"));
	//Engine::get_singleton()->get_singleton("ResourceFormatLoaderResourceImage")->call("get_recognized_extensions");

	//Array split_path = r_path.get_basename().split("*");
	//if (split_path.size() == 2) {
	//	r_found = true;
	//	return;
	//}
	String file = r_p_path.get_base_dir() + r_path.get_file();
	if (dir->file_exists(file)) {
		r_path = r_p_path.get_base_dir() + r_path.get_file();
		r_found = true;
		return;
	}

	for (int32_t i = 0; i < exts.size(); i++) {
		if (r_found) {
			return;
		}
		if (r_found == false) {
			_find_texture_path(r_p_path, dir, r_path, r_found, String(".") + exts[i]);
		}
	}
}

void ComChibifireFbxImporter::_find_texture_path(const String &p_path, godot::Directory *dir, String &path, bool &found, String extension) {
	String name = path.get_basename() + extension;
	if (dir->file_exists(name)) {
		found = true;
		path = name;
		return;
	}
	String name_ignore_sub_directory = p_path.get_base_dir() + "/" + path.get_file().get_basename() + extension;
	if (dir->file_exists(name_ignore_sub_directory)) {
		found = true;
		path = name_ignore_sub_directory;
		return;
	}

	String name_find_texture_sub_directory = p_path.get_base_dir() + "/textures/" + path.get_file().get_basename() + extension;
	if (dir->file_exists(name_find_texture_sub_directory)) {
		found = true;
		path = name_find_texture_sub_directory;
		return;
	}
	String name_find_texture_upper_sub_directory = p_path.get_base_dir() + "/Textures/" + path.get_file().get_basename() + extension;
	if (dir->file_exists(name_find_texture_upper_sub_directory)) {
		found = true;
		path = name_find_texture_upper_sub_directory;
		return;
	}
	String name_find_texture_outside_sub_directory = p_path.get_base_dir() + "/../textures/" + path.get_file().get_basename() + extension;
	if (dir->file_exists(name_find_texture_outside_sub_directory)) {
		found = true;
		path = name_find_texture_outside_sub_directory;
		return;
	}

	String name_find_upper_texture_outside_sub_directory = p_path.get_base_dir() + "/../Textures/" + path.get_file().get_basename() + extension;
	if (dir->file_exists(name_find_upper_texture_outside_sub_directory)) {
		found = true;
		path = name_find_upper_texture_outside_sub_directory;
		return;
	}
}

Ref<godot::Material> ComChibifireFbxImporter::_generate_material_from_index(const ImportState &p_state, const RawMaterial p_raw_material, const RawMaterialType p_raw_material_type) {
	Ref<SpatialMaterial> mat = SpatialMaterial::_new();
	mat->set_name(p_raw_material.name.c_str());

	//int32_t mat_two_sided = 0;
	//if (p_raw_material.userProperties) {
	//	if (mat_two_sided > 0) {
	//		mat->set_cull_mode(SpatialMaterial::CULL_DISABLED);
	//	}
	//}

	if (p_raw_material_type == RAW_MATERIAL_TYPE_TRANSPARENT || p_raw_material_type == RAW_MATERIAL_TYPE_SKINNED_TRANSPARENT) {
		mat->set_feature(SpatialMaterial::FEATURE_TRANSPARENT, true);
		mat->set_depth_draw_mode(SpatialMaterial::DepthDrawMode::DEPTH_DRAW_ALPHA_OPAQUE_PREPASS);
	}

	Array user_properties;
	for (size_t i = 0; i < p_raw_material.userProperties.size(); i++) {
		String prop = String(p_raw_material.userProperties[i].c_str());
		user_properties.push_back(prop);
	}

	if (p_raw_material.info->shadingModel != RAW_SHADING_MODEL_PBR_MET_ROUGH) {

		//TODO(Ernest) Legacy materials
		//auto simpleTex = [&](RawTextureUsage usage) -> std::shared_ptr<TextureData> {
		//	return (p_raw_material.textures[usage] >= 0) ? simple(p_raw_material.textures[usage], "simple") : nullptr;
		//};
		Vec4f diffuseFactor;
		float metallic, roughness;
		std::vector<ComChibifireFbxImporter::TexInfo> aoMetRoughTex;
		const RawTraditionalMatProps *props = ((RawTraditionalMatProps *)p_raw_material.info.get());
		diffuseFactor = props->diffuseFactor;

		if (p_raw_material.info->shadingModel == RAW_SHADING_MODEL_BLINN ||
				p_raw_material.info->shadingModel == RAW_SHADING_MODEL_PHONG) {
			// blinn/phong hardcoded to 0.4 metallic
			metallic = 0.4f;

			// fairly arbitrary conversion equation, with properties:
			//   shininess 0 -> roughness 1
			//   shininess 2 -> roughness ~0.7
			//   shininess 6 -> roughness 0.5
			//   shininess 16 -> roughness ~0.33
			//   as shininess ==> oo, roughness ==> 0
			auto getRoughness = [&](float shininess) { return sqrtf(2.0f / (2.0f + shininess)); };

			aoMetRoughTex = combine(
					*p_state.scene,
					{
							p_raw_material.textures[RAW_TEXTURE_USAGE_SHININESS],
					},
					"ao_met_rough",
					[&](const std::vector<const TextureBuilder::pixel *> pixels)
							-> TextureBuilder::pixel {
						// do not multiply with props->shininess; that doesn't work like the other
						// factors.
						float shininess = props->shininess * (*pixels[0])[0];
						return { { 0, getRoughness(shininess), metallic, 1 } };
					},
					false);

			if (aoMetRoughTex.size() != 0) {
				// if we successfully built a texture, factors are just multiplicative identity
				metallic = roughness = 1.0f;

				Ref<Image> img;
				img.instance();
				PoolByteArray arr;
				uint32_t size = aoMetRoughTex[0].width * aoMetRoughTex[0].height;
				arr.resize(size);
				memcpy(arr.write().ptr(), aoMetRoughTex[0].pixels, size);
				ERR_FAIL_COND_V(arr.size() % 4 != 0, Ref<Texture>());
				//ARGB8888 to RGBA8888
				//for (int32_t i = 0; i < arr.size() / 4; i++) {
				//	arr.write().ptr()[(4 * i) + 3] = arr[(4 * i) + 0];
				//	arr.write().ptr()[(4 * i) + 0] = arr[(4 * i) + 1];
				//	arr.write().ptr()[(4 * i) + 1] = arr[(4 * i) + 2];
				//	arr.write().ptr()[(4 * i) + 2] = arr[(4 * i) + 3];
				//}
				img->create_from_data(aoMetRoughTex[0].width, aoMetRoughTex[0].height, true, Image::FORMAT_RGBA8, arr);
				ERR_FAIL_COND_V(img.is_null(), Ref<Texture>());

				Ref<ImageTexture> t;
				t.instance();
				t->create_from_image(img);
				t->set_storage(ImageTexture::STORAGE_COMPRESS_LOSSY);
				mat->set_texture(SpatialMaterial::TEXTURE_METALLIC, t);
				mat->set_texture(SpatialMaterial::TEXTURE_ROUGHNESS, t);
				mat->set_texture(SpatialMaterial::TEXTURE_AMBIENT_OCCLUSION, t);

			} else {
				// no shininess texture,
				roughness = getRoughness(props->shininess);
			}

		} else {
			metallic = 0.2f;
			roughness = 0.8f;
		}
		mat->set_metallic(metallic);
		mat->set_roughness(roughness);
	}

	if (p_raw_material.textures[RAW_TEXTURE_USAGE_OCCLUSION] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_OCCLUSION]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_feature(SpatialMaterial::FEATURE_AMBIENT_OCCLUSION, true);
				mat->set_texture(SpatialMaterial::TEXTURE_AMBIENT_OCCLUSION, texture);
			}
		}
	}

	if (p_raw_material.textures[RAW_TEXTURE_USAGE_NORMAL] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_NORMAL]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_feature(SpatialMaterial::Feature::FEATURE_NORMAL_MAPPING, true);
				mat->set_texture(SpatialMaterial::TEXTURE_NORMAL, texture);
			}
		}
	}

	if (p_raw_material.textures[RAW_TEXTURE_USAGE_EMISSIVE] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_EMISSIVE]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_feature(SpatialMaterial::FEATURE_EMISSION, true);
				mat->set_texture(SpatialMaterial::TEXTURE_EMISSION, texture);
			}
		}
	}
	if (p_raw_material.textures[RAW_TEXTURE_USAGE_DIFFUSE] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_DIFFUSE]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_texture(SpatialMaterial::TEXTURE_ALBEDO, texture);
			}
		}
	}
	if (p_raw_material.textures[RAW_TEXTURE_USAGE_ROUGHNESS] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_ROUGHNESS]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_texture(SpatialMaterial::TEXTURE_ROUGHNESS, texture);
			}
		}
	}
	if (p_raw_material.textures[RAW_TEXTURE_USAGE_METALLIC] != -1) {
		const RawTexture raw_texture = p_state.scene->GetTexture(p_raw_material.textures[RAW_TEXTURE_USAGE_METALLIC]);
		String filename = raw_texture.fileName.c_str();
		String path = raw_texture.fileLocation.c_str();
		bool found = false;
		_find_texture_path(p_state.path, path, found);
		if (found) {
			Ref<Texture> texture = _load_texture(p_state, path);

			if (texture != NULL) {
				mat->set_texture(SpatialMaterial::TEXTURE_ROUGHNESS, texture);
			}
		}
	}
	return mat;
}

Ref<godot::Texture> ComChibifireFbxImporter::_load_texture(const ImportState &p_state, const String p_path) {
	Ref<Texture> p_texture = ResourceLoader::get_singleton()->load(p_path, "Texture");
	return p_texture;
}

std::vector<ComChibifireFbxImporter::TexInfo> ComChibifireFbxImporter::combine(const RawModel &p_scene, const std::vector<int> &ixVec, const std::string &tag, const pixel_merger &mergeFunction, bool transparency) {
	int width = -1, height = -1;
	std::string mergedFilename = tag;
	std::vector<TexInfo> texes{};
	for (const int rawTexIx : ixVec) {
		TexInfo info(rawTexIx);
		if (rawTexIx >= 0) {
			const RawTexture &rawTex = p_scene.GetTexture(rawTexIx);
			const std::string &fileLoc = rawTex.fileLocation;
			const std::string &name =
					StringUtils::GetFileBaseString(StringUtils::GetFileNameString(fileLoc));
			if (!fileLoc.empty()) {
				info.pixels = stbi_load(fileLoc.c_str(), &info.width, &info.height, &info.channels, 0);
				if (!info.pixels) {
					fmt::printf("Warning: merge texture [%d](%s) could not be loaded.\n", rawTexIx, name);
				} else {
					if (width < 0) {
						width = info.width;
						height = info.height;
					} else if (width != info.width || height != info.height) {
						fmt::printf(
								"Warning: texture %s (%d, %d) can't be merged with previous texture(s) of dimension (%d, %d)\n",
								name,
								info.width,
								info.height,
								width,
								height);
						// this is bad enough that we abort the whole merge
						return std::vector<TexInfo>();
					}
					mergedFilename += "_" + name;
				}
			}
		}
		texes.push_back(info);
	}
	// at the moment, the best choice of filename is also the best choice of name
	const std::string mergedName = mergedFilename;

	if (width < 0) {
		// no textures to merge; bail
		return std::vector<TexInfo>();
	}
	// TODO: which channel combinations make sense in input files?

	// write 3 or 4 channels depending on whether or not we need transparency
	int channels = transparency ? 4 : 3;

	std::vector<uint8_t> mergedPixels(static_cast<size_t>(channels * width * height));
	std::vector<pixel> pixels(texes.size());
	std::vector<const pixel *> pixelPointers(texes.size());
	for (int xx = 0; xx < width; xx++) {
		for (int yy = 0; yy < height; yy++) {
			pixels.clear();
			for (int jj = 0; jj < texes.size(); jj++) {
				const TexInfo &tex = texes[jj];
				// each texture's structure will depend on its channel count
				int ii = tex.channels * (xx + yy * width);
				int kk = 0;
				if (tex.pixels != nullptr) {
					for (; kk < tex.channels; kk++) {
						pixels[jj][kk] = tex.pixels[ii++] / 255.0f;
					}
				}
				for (; kk < pixels[jj].size(); kk++) {
					pixels[jj][kk] = 1.0f;
				}
				pixelPointers[jj] = &pixels[jj];
			}
			const pixel merged = mergeFunction(pixelPointers);
			int ii = channels * (xx + yy * width);
			for (int jj = 0; jj < channels; jj++) {
				mergedPixels[ii + jj] = static_cast<uint8_t>(fmax(0, fmin(255.0f, merged[jj] * 255.0f)));
			}
		}
	}
	return texes;
}
