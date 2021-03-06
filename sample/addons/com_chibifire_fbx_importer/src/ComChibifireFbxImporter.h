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
	void _generate_material(std::vector<RawModel> material_models, godot::Spatial *root, godot::Dictionary &mesh_cache, ImportState state);
	godot::Ref<godot::Animation> _import_animation(const String path, const int64_t flags, const int64_t bake_fps);
	static void _register_methods();
};
