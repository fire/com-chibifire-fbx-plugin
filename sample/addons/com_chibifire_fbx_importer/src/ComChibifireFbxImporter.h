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

#pragma once

#include <Animation.hpp>
#include <ArrayMesh.hpp>
#include <Directory.hpp>
#include <EditorPlugin.hpp>
#include <EditorSceneImporter.hpp>
#include <Engine.hpp>
#include <Material.hpp>
#include <MeshInstance.hpp>
#include <Node.hpp>
#include <Ref.hpp>
#include <Reference.hpp>
#include <Skeleton.hpp>
#include <String.hpp>
#include <Texture.hpp>
#include <Transform.hpp>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>
#include <map>
#include <raw/RawModel.hpp>
#include <vector>

using godot::Array;
using godot::ArrayMesh;
using godot::Dictionary;
using godot::EditorSceneImporter;
using godot::Node;
using godot::Ref;
using godot::Reference;
using godot::register_class;
using godot::String;
using godot::Variant;

enum PrimitiveType {
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES = 1,
	PRIMITIVE_LINE_STRIP = 2,
	PRIMITIVE_LINE_LOOP = 3,
	PRIMITIVE_TRIANGLES = 4,
	PRIMITIVE_TRIANGLE_STRIP = 5,
	PRIMITIVE_TRIANGLE_FAN = 6,
	PRIMITIVE_MAX = 7,
};

enum ArrayType {
	ARRAY_VERTEX = 0,
	ARRAY_NORMAL = 1,
	ARRAY_TANGENT = 2,
	ARRAY_COLOR = 3,
	ARRAY_TEX_UV = 4,
	ARRAY_TEX_UV2 = 5,
	ARRAY_BONES = 6,
	ARRAY_WEIGHTS = 7,
	ARRAY_INDEX = 8,
	ARRAY_MAX = 9
};

enum Presets {
	PRESET_SEPARATE_MATERIALS,
	PRESET_SEPARATE_MESHES,
	PRESET_SEPARATE_ANIMATIONS,

	PRESET_SINGLE_SCENE,

	PRESET_SEPARATE_MESHES_AND_MATERIALS,
	PRESET_SEPARATE_MESHES_AND_ANIMATIONS,
	PRESET_SEPARATE_MATERIALS_AND_ANIMATIONS,
	PRESET_SEPARATE_MESHES_MATERIALS_AND_ANIMATIONS,

	PRESET_MULTIPLE_SCENES,
	PRESET_MULTIPLE_SCENES_AND_MATERIALS,
	PRESET_MAX
};

class ComChibifireFbxImporter : public EditorSceneImporter {
private:
	GODOT_CLASS(ComChibifireFbxImporter, EditorSceneImporter);
	struct ImportState {
		const RawModel *scene;
		const godot::String path;
		std::vector<godot::Skeleton *> skeletons;
		std::map<godot::String, int> bone_owners; //maps bones to skeleton index owned by
		std::map<godot::MeshInstance *, godot::Skeleton *> mesh_skeletons;
	};
	struct SkeletonHole { //nodes may be part of the skeleton by used by vertex
		String name;
		String parent;
		godot::Transform pose;
		const RawNode &node;
	};
	void _generate_bone_groups(ImportState &p_state, const RawNode &p_node, std::map<String, int> &p_ownership, std::map<String, godot::Transform> &p_bind_xforms);
	void _generate_skeletons(ImportState &p_state, const RawNode &p_node, std::map<String, int>& ownership, Dictionary& skeleton_map, std::map<String, godot::Transform>& bind_xforms);
	void _generate_node(ImportState &p_state, const RawNode &p_node, Node *p_parent, Node *p_owner, Array &r_bone_name);
	String _convert_name(const std::string str);
	godot::Transform _get_transform(Quatf rotation, Vec3f scale, Vec3f translation);
	void _find_texture_path(const String &r_p_path, String &r_path, bool &r_found);
	void _find_texture_path(const String &p_path, godot::Directory *dir, String &path, bool &found, String extension);
	Ref<godot::Material> _generate_material_from_index(const ImportState &p_state, const RawMaterial p_raw_material, const RawMaterialType p_raw_material_type);
	Ref<godot::Texture> _load_texture(const ImportState &p_state, const String p_path);
	using pixel = std::array<float, 4>; // pixel components are floats in [0, 1]
	using pixel_merger = std::function<pixel(const std::vector<const pixel *>)>;
	struct TexInfo {
		explicit TexInfo(int rawTexIx) :
				rawTexIx(rawTexIx) {}

		const int rawTexIx;
		int width{};
		int height{};
		int channels{};
		uint8_t *pixels{};
	};
	std::vector<TexInfo> combine(
			const RawModel &p_scene,
			const std::vector<int> &ixVec,
			const std::string &tag,
			const pixel_merger &mergeFunction,
			bool transparency);
	godot::Transform _get_global_node_transform(ImportState &p_state, const RawNode &p_node);
	void _fill_node_relationships(ImportState &p_state, const RawNode *p_node, std::map<String, int>&ownership, Dictionary &skeleton_map, int p_skeleton_id, godot::Skeleton &p_skeleton, String &parent_name, int &holecount, std::vector<SkeletonHole>& p_holes, std::map<String, godot::Transform>& bind_xforms);

public:
	enum ImportFlags {
		IMPORT_SCENE = 1,
		IMPORT_ANIMATION = 2,
		IMPORT_ANIMATION_DETECT_LOOP = 4,
		IMPORT_ANIMATION_OPTIMIZE = 8,
		IMPORT_ANIMATION_FORCE_ALL_TRACKS_IN_ALL_CLIPS = 16,
		IMPORT_ANIMATION_KEEP_VALUE_TRACKS = 32,
		IMPORT_GENERATE_TANGENT_ARRAYS = 256,
		IMPORT_FAIL_ON_MISSING_DEPENDENCIES = 512,
		IMPORT_MATERIALS_IN_INSTANCES = 1024,
		IMPORT_USE_COMPRESSION = 2048
	};

	ComChibifireFbxImporter() {}

	void _init() {}

	Array get_extensions() const;
	int64_t get_import_flags() const;
	Node *_import_scene(const String path, const int64_t flags, const int64_t bake_fps);

	void _generate_material(std::vector<RawModel> material_models, Spatial *root, Dictionary &mesh_cache, ImportState state) {
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
	}

	godot::Ref<godot::Animation> _import_animation(const String path, const int64_t flags, const int64_t bake_fps);

	static void _register_methods();
};
