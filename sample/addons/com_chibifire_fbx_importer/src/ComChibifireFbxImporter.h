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
#include <EditorPlugin.hpp>
#include <EditorSceneImporter.hpp>
#include <Node.hpp>
#include <Ref.hpp>
#include <Reference.hpp>
#include <String.hpp>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>
#include "raw/RawModel.hpp"

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
	Node *import_scene(const String path, const int64_t flags, const int64_t bake_fps);
	godot::Ref<godot::Animation> import_animation(const String path, const int64_t flags, const int64_t bake_fps);

	static void _register_methods();

//	
//void _get_axis_angle(godot::Basis basis, godot::Vector3 &r_axis, real_t &r_angle) const {
//
//		if (!basis.is_rotation()) {
//			Godot::print("!is_rotation()");
//			return;
//		}
//
//		real_t angle, x, y, z; // variables for result
//		real_t epsilon = 0.01; // margin to allow for rounding errors
//		real_t epsilon2 = 0.1; // margin to distinguish between 0 and 180 degrees
//		Basis elements = basis;
//		if ((abs(elements[1][0] - elements[0][1]) < epsilon) && (abs(elements[2][0] - elements[0][2]) < epsilon) && (abs(elements[2][1] - elements[1][2]) < epsilon)) {
//			// singularity found
//			// first check for identity matrix which must have +1 for all terms
//			//  in leading diagonaland zero in other terms
//			if ((abs(elements[1][0] + elements[0][1]) < epsilon2) && (abs(elements[2][0] + elements[0][2]) < epsilon2) && (abs(elements[2][1] + elements[1][2]) < epsilon2) && (abs(elements[0][0] + elements[1][1] + elements[2][2] - 3) < epsilon2)) {
//				// this singularity is identity matrix so angle = 0
//				r_axis = Vector3(0, 1, 0);
//				r_angle = 0;
//				return;
//			}
//			// otherwise this singularity is angle = 180
//			angle = Math_PI;
//			real_t xx = (elements[0][0] + 1) / 2;
//			real_t yy = (elements[1][1] + 1) / 2;
//			real_t zz = (elements[2][2] + 1) / 2;
//			real_t xy = (elements[1][0] + elements[0][1]) / 4;
//			real_t xz = (elements[2][0] + elements[0][2]) / 4;
//			real_t yz = (elements[2][1] + elements[1][2]) / 4;
//			if ((xx > yy) && (xx > zz)) { // elements[0][0] is the largest diagonal term
//				if (xx < epsilon) {
//					x = 0;
//					y = 0.7071;
//					z = 0.7071;
//				} else {
//					x = sqrt(xx);
//					y = xy / x;
//					z = xz / x;
//				}
//			} else if (yy > zz) { // elements[1][1] is the largest diagonal term
//				if (yy < epsilon) {
//					x = 0.7071;
//					y = 0;
//					z = 0.7071;
//				} else {
//					y = sqrt(yy);
//					x = xy / y;
//					z = yz / y;
//				}
//			} else { // elements[2][2] is the largest diagonal term so base result on this
//				if (zz < epsilon) {
//					x = 0.7071;
//					y = 0.7071;
//					z = 0;
//				} else {
//					z = sqrt(zz);
//					x = xz / z;
//					y = yz / z;
//				}
//			}
//			r_axis = Vector3(x, y, z);
//			r_angle = angle;
//			return;
//		}
//		// as we have reached here there are no singularities so we can handle normally
//		real_t s = sqrt((elements[1][2] - elements[2][1]) * (elements[1][2] - elements[2][1]) + (elements[2][0] - elements[0][2]) * (elements[2][0] - elements[0][2]) + (elements[0][1] - elements[1][0]) * (elements[0][1] - elements[1][0])); // s=|axis||sin(angle)|, used to normalise
//
//		angle = acos((elements[0][0] + elements[1][1] + elements[2][2] - 1) / 2);
//		if (angle < 0) s = -s;
//		x = (elements[2][1] - elements[1][2]) / s;
//		y = (elements[0][2] - elements[2][0]) / s;
//		z = (elements[1][0] - elements[0][1]) / s;
//
//		r_axis = Vector3(x, y, z);
//		r_angle = angle;
//	}

	//Spatial *_find_armature(const aiScene *scene, const Skeleton *s, const Array bone_names) {

	//	aiNode *current_bone = scene->mRootNode->FindNode(_string_to_ai_string(s->get_bone_name(s->get_bone_count() - 1)));
	//	String name = _ai_string_to_string(current_bone->mParent->mName);
	//	while (current_bone != NULL && current_bone->mParent != NULL && bone_names.has(name) != false) {
	//		current_bone = scene->mRootNode->FindNode(current_bone->mName)->mParent;
	//	}
	//	if (current_bone == NULL) {
	//		return NULL;
	//	}
	//	return Spatial::cast_to<Spatial>(s->get_owner()->find_node(_ai_string_to_string(current_bone->mName)));
	//}

	//Transform _get_armature_xform(const aiScene *scene, const Skeleton *s, const Array bone_names, const Spatial *root, const Spatial *p_mesh_instance) {

	//	Spatial *armature_node = _find_armature(scene, s, bone_names);

	//	if (armature_node == NULL) {
	//		return Transform();
	//	}
	//	if (armature_node->is_a_parent_of(p_mesh_instance) == false) {
	//		return Transform();
	//	}
	//	return _get_global_ai_node_transform(scene, scene->mRootNode->FindNode(_string_to_ai_string(armature_node->get_name())));
	//}

	//aiString _string_to_ai_string(String bone_name) {
	//	//https://stackoverflow.com/a/12903901/381724
	//	//https://godotengine.org/qa/18552/gdnative-convert-godot-string-to-const-char

	//	aiString string;
	//	string.Set(bone_name.alloc_c_string());
	//	return string;
	//}

	//Transform _get_global_ai_node_transform(const aiScene *p_scene, const aiNode *p_current_node) {
	//	aiNode const *current_node = p_current_node;
	//	Transform xform;
	//	while (current_node != NULL) {
	//		if (_extract_ai_matrix_transform(current_node->mTransformation) != Transform()) {
	//			xform = _extract_ai_matrix_transform(current_node->mTransformation) * xform;
	//		}
	//		current_node = current_node->mParent;
	//		xform.orthonormalize();
	//	}
	//	return xform;
	//}

	//void _generate_node_bone(const String &p_path, RawModel& p_scene, const aiNode *p_node, Node *p_owner, Array &p_skeletons, Array &r_bone_name, Array p_light_names, Array p_camera_names) {
	//	bool enable_hack = true;
	//	if (enable_hack) {
	//		for (size_t k = 0; k < p_skeletons.size(); k++) {
	//			Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(p_skeletons[k]));
	//			String name = _ai_string_to_string(p_node->mName);
	//			bool can_create_bone = name != _ai_string_to_string(p_scene->mRootNode->mName) && p_node->mNumChildren > 0 && p_node->mNumMeshes == 0 && p_camera_names.has(name) == false && p_light_names.has(name) == false;
	//			if (can_create_bone && r_bone_name.has(name) == false) {
	//				s->add_bone(name);
	//				r_bone_name.push_back(name);
	//				int32_t idx = s->find_bone(name);
	//				Transform bone_offset = _get_global_ai_node_transform(p_scene, p_node);
	//				s->set_bone_rest(idx, bone_offset);
	//			}
	//			p_skeletons[k] = s;
	//		}
	//	}

	//	for (size_t i = 0; i < p_node->mNumMeshes; i++) {
	//		for (size_t k = 0; k < p_skeletons.size(); k++) {
	//			Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(p_skeletons[k]));
	//			const unsigned int mesh_idx = p_node->mMeshes[i];
	//			const aiMesh *ai_mesh = p_scene->mMeshes[mesh_idx];
	//			for (int j = 0; j < ai_mesh->mNumBones; j++) {
	//				String bone_name = _ai_string_to_string(ai_mesh->mBones[j]->mName);
	//				bool has_bone = s->find_bone(bone_name) != -1;
	//				if (has_bone) {
	//					continue;
	//				}
	//				s->add_bone(bone_name);
	//				if (r_bone_name.has(bone_name) == false) {
	//					r_bone_name.push_back(bone_name);
	//				}
	//			}
	//			for (int l = 0; l < ai_mesh->mNumBones; l++) {
	//				String bone_name = _ai_string_to_string(ai_mesh->mBones[l]->mName);
	//				int32_t bone_idx = s->find_bone(bone_name);
	//				if (bone_idx == -1) {
	//					continue;
	//				}
	//				Transform bone_offset = _extract_ai_matrix_transform(ai_mesh->mBones[l]->mOffsetMatrix).affine_inverse();
	//				s->set_bone_rest(bone_idx, bone_offset);
	//			}
	//			p_skeletons[k] = s;
	//		}
	//	}

	//	for (int i = 0; i < p_node->mNumChildren; i++) {
	//		_generate_node_bone(raw, p_node->mChildren[i], p_owner, p_skeletons, r_bone_name);
	//	}
	//}

	void _generate_node(const RawModel& p_scene, const RawNode& p_node, Node *p_parent, Node *p_owner, Array &p_skeletons, Array &r_bone_name);

	//bool _add_mesh_to_mesh_instance(const aiNode *p_node, const aiScene *p_scene, bool has_uvs, Skeleton *s, const String &p_path, MeshInstance *p_mesh_instance, Node *p_owner, Array &r_bone_name) {
	//	ArrayMesh *mesh = ArrayMesh::_new();

	//	for (size_t i = 0; i < p_node->mNumMeshes; i++) {
	//		const unsigned int mesh_idx = p_node->mMeshes[i];
	//		const aiMesh *ai_mesh = p_scene->mMeshes[mesh_idx];
	//		p_mesh_instance->set_name(_ai_string_to_string(p_node->mName));
	//		Godot::print("Open Asset Importer: Creating mesh for: " + p_mesh_instance->get_name());
	//		SurfaceTool *st = SurfaceTool::_new();
	//		st->begin(Mesh::PRIMITIVE_TRIANGLES);
	//		Dictionary vertex_weight;
	//		Dictionary vertex_bone_name;
	//		for (size_t b = 0; b < ai_mesh->mNumBones; b++) {
	//			aiBone *bone = ai_mesh->mBones[b];
	//			for (size_t w = 0; w < bone->mNumWeights; w++) {
	//				String name = _ai_string_to_string(bone->mName);
	//				aiVertexWeight ai_weights = bone->mWeights[w];
	//				uint32_t vertexId = ai_weights.mVertexId;
	//				Array result = vertex_weight[vertexId];
	//				Array weights;
	//				if (result.size()) {
	//					weights = result;
	//				}
	//				weights.push_back(ai_weights.mWeight);
	//				if (vertex_weight.has(vertexId)) {
	//					vertex_weight[vertexId] = weights;
	//				} else {
	//					vertex_weight[vertexId] = weights;
	//				}
	//				Array bone_result = vertex_bone_name[vertexId];
	//				Array bone_names;
	//				if (bone_result.size()) {
	//					bone_names = bone_result;
	//				}
	//				bone_names.push_back(name);
	//				if (vertex_bone_name.has(vertexId)) {
	//					vertex_bone_name[vertexId] = bone_names;
	//				} else {
	//					vertex_bone_name[vertexId] = bone_names;
	//				}
	//			}
	//		}
	//		int32_t surface_flags;
	//		for (size_t j = 0; j < ai_mesh->mNumFaces; j++) {
	//			const aiFace face = ai_mesh->mFaces[j];
	//			Array tri_order;
	//			tri_order.push_back(0);
	//			tri_order.push_back(2);
	//			tri_order.push_back(1);
	//			for (size_t k = 0; k < face.mNumIndices; k++) {
	//				if (k >= tri_order.size()) {
	//					return has_uvs;
	//				}
	//				int32_t tri = tri_order[k];
	//				uint32_t index = face.mIndices[tri];
	//				if (ai_mesh->HasTextureCoords(0)) {
	//					has_uvs = true;
	//					surface_flags = surface_flags | Mesh::ARRAY_FORMAT_TEX_UV;
	//					st->add_uv(Vector2(ai_mesh->mTextureCoords[0][index].x, 1.0f - ai_mesh->mTextureCoords[0][index].y));
	//				}
	//				if (ai_mesh->HasTextureCoords(1)) {
	//					has_uvs = true;
	//					surface_flags = surface_flags | Mesh::ARRAY_FORMAT_TEX_UV2;
	//					st->add_uv2(Vector2(ai_mesh->mTextureCoords[1][index].x, 1.0f - ai_mesh->mTextureCoords[1][index].y));
	//				}
	//				const aiVector3D normals = ai_mesh->mNormals[index];
	//				const Vector3 godot_normal = _vec3_convert_to_godot(Vector3(normals.x, normals.y, normals.z));
	//				st->add_normal(godot_normal);
	//				if (ai_mesh->HasTangentsAndBitangents()) {
	//					const aiVector3D tangents = ai_mesh->mTangents[index];
	//					const Vector3 godot_tangent = _vec3_convert_to_godot(Vector3(tangents.x, tangents.y, tangents.z));
	//					const aiVector3D bitangent = ai_mesh->mBitangents[index];
	//					const Vector3 godot_bitangent = _vec3_convert_to_godot(Vector3(bitangent.x, bitangent.y, bitangent.z));
	//					float d = godot_normal.cross(godot_tangent).dot(godot_bitangent) > 0.0f ? 1.0f : -1.0f;
	//					st->add_tangent(Plane(tangents.x, tangents.y, tangents.z, d));
	//				}

	//				Array I = vertex_bone_name[index];
	//				Array bones;

	//				if (I.size()) {
	//					Array bone_names;
	//					bone_names = I;
	//					for (size_t f = 0; f < bone_names.size(); f++) {
	//						int32_t bone = s->find_bone(bone_names[f]);
	//						if (bone == -1) {
	//							return has_uvs;
	//						}
	//						bones.push_back(bone);
	//					}
	//					if (s->get_bone_count()) {
	//						int32_t add = CLAMP(VisualServer::ARRAY_WEIGHTS_SIZE - bones.size(), 0, VisualServer::ARRAY_WEIGHTS_SIZE);
	//						for (size_t f = 0; f < add; f++) {
	//							bones.push_back(s->get_bone_count() - 1);
	//						}
	//					}
	//					st->add_bones(bones);
	//					Array E = vertex_weight[index];
	//					Array weights;
	//					if (E.size()) {
	//						weights = E;
	//						if (weights.size() < VisualServer::ARRAY_WEIGHTS_SIZE) {
	//							int32_t add = CLAMP(VisualServer::ARRAY_WEIGHTS_SIZE - weights.size(), 0, VisualServer::ARRAY_WEIGHTS_SIZE);
	//							for (size_t f = 0; f < add; f++) {
	//								weights.push_back(0.0f);
	//							}
	//						}
	//					}
	//					if (weights.size() == 0) {
	//						continue;
	//					}
	//					st->add_weights(weights);
	//				} else if (s->get_bone_count() > 0) {
	//					for (size_t f = 0; f < VisualServer::ARRAY_WEIGHTS_SIZE; f++) {
	//						String bone_name = s->get_bone_name(s->get_bone_count() - 1);
	//						int32_t bone = s->find_bone(bone_name);
	//						ERR_FAIL_COND_V(bone == -1, has_uvs);
	//						bones.push_back(bone);
	//					}
	//					st->add_bones(bones);
	//					Array empty_weights;
	//					for (size_t w = 0; w < VisualServer::ARRAY_WEIGHTS_SIZE; w++) {
	//						empty_weights.push_back(1.0f / VisualServer::ARRAY_WEIGHTS_SIZE);
	//					}
	//					st->add_weights(empty_weights);
	//				}
	//				const aiVector3D pos = ai_mesh->mVertices[index];
	//				Vector3 godot_pos = _vec3_convert_to_godot(Vector3(pos.x, pos.y, pos.z));
	//				st->add_vertex(godot_pos);
	//			}
	//		}

	//		st->index();
	//		surface_flags |= Mesh::ARRAY_FORMAT_VERTEX | Mesh::ARRAY_FORMAT_INDEX | Mesh::ARRAY_BONES | Mesh::ARRAY_FORMAT_WEIGHTS;
	//		if (has_uvs) {
	//			surface_flags |= Mesh::ARRAY_FORMAT_NORMAL | Mesh::ARRAY_FORMAT_TANGENT;
	//		}
	//		if (ai_mesh->HasTangentsAndBitangents() == false) {
	//			st->generate_tangents();
	//		}
	//		aiMaterial *ai_material = p_scene->mMaterials[ai_mesh->mMaterialIndex];
	//		SpatialMaterial *mat = SpatialMaterial::_new();
	//		mat->set_name(_ai_string_to_string(ai_material->GetName()));
	//		Dictionary properties;
	//		for (size_t p = 0; p < ai_material->mNumProperties; p++) {
	//			aiMaterialProperty *property = ai_material->mProperties[p];

	//			String key = _ai_string_to_string(property->mKey);
	//			properties[key] = p;
	//		}
	//		if (properties.has("$clr.transparent")) {
	//			mat->set_feature(SpatialMaterial::Feature::FEATURE_TRANSPARENT, true);
	//			mat->set_depth_draw_mode(SpatialMaterial::DepthDrawMode::DEPTH_DRAW_ALPHA_OPAQUE_PREPASS);
	//		}
	//		_load_material_type(SpatialMaterial::TEXTURE_ALBEDO, aiTextureType_DIFFUSE, mat, ai_material, p_path);
	//		_load_material_type(SpatialMaterial::TEXTURE_EMISSION, aiTextureType_EMISSIVE, mat, ai_material, p_path);
	//		_load_material_type(SpatialMaterial::TEXTURE_NORMAL, aiTextureType_NORMALS, mat, ai_material, p_path);

	//		st->set_material(Ref<Material>(mat));
	//		p_mesh_instance->set_mesh(st->commit(mesh).ptr());
	//	}
	//	//for (int i = 0; i < mesh.blend_weights.size(); i++) {
	//	//	mi->set("blend_shapes/" + mesh.mesh->get_blend_shape_name(i), mesh.blend_weights[i]);
	//	//}
	//	return has_uvs;
	//}

	//void _load_material_type(int64_t p_spatial_material_type, aiTextureType p_texture_type, SpatialMaterial *p_spatial_material, aiMaterial *p_ai_material, const String &p_path) {
	//	for (size_t t = 0; t < p_ai_material->GetTextureCount(p_texture_type); t++) {
	//		if (p_spatial_material_type == SpatialMaterial::TEXTURE_NORMAL) {
	//			p_spatial_material->set_feature(SpatialMaterial::Feature::FEATURE_NORMAL_MAPPING, true);
	//		}
	//		if (p_spatial_material_type == SpatialMaterial::TEXTURE_EMISSION) {
	//			p_spatial_material->set_feature(SpatialMaterial::Feature::FEATURE_EMISSION, true);
	//		}
	//		aiString texture_path;
	//		p_ai_material->GetTexture(p_texture_type, t, &texture_path);
	//		String path = p_path.get_base_dir() + "/" + _ai_string_to_string(texture_path).replace("\\", "/");
	//		Directory *dir = Directory::_new();
	//		bool found = false;
	//		if (dir->file_exists(p_path.get_base_dir() + path)) {
	//			found = true;
	//		}
	//		if (found == false) {
	//			found = _find_texture_path(p_path, dir, path, found, ".jpg");
	//		}
	//		if (found == false) {
	//			found = _find_texture_path(p_path, dir, path, found, ".jpeg");
	//		}
	//		if (found == false) {
	//			found = _find_texture_path(p_path, dir, path, found, ".png");
	//		}
	//		if (found == false) {
	//			found = _find_texture_path(p_path, dir, path, found, ".exr");
	//		}
	//		if (found == false) {
	//			found = _find_texture_path(p_path, dir, path, found, ".tga");
	//		}
	//		if (found == false) {
	//			continue;
	//		}
	//		Ref<Texture> texture = ResourceLoader::get_singleton()->load(path, "Texture");
	//		p_spatial_material->set_texture(p_spatial_material_type, texture);
	//	}
	//}

	//bool _find_texture_path(const String &p_path, Directory *dir, String &path, bool &found, String extension) {
	//	String name = path.get_basename() + extension;
	//	if (dir->file_exists(name)) {
	//		found = true;
	//		path = name;
	//		return found;
	//	}
	//	String name_ignore_sub_directory = p_path.get_base_dir() + "/" + path.get_file().get_basename() + extension;
	//	if (dir->file_exists(name_ignore_sub_directory)) {
	//		found = true;
	//		path = name_ignore_sub_directory;
	//		return found;
	//	}
	//	return found;
	//}

	//String _ai_string_to_string(const aiString p_node) {
	//	String name = p_node.C_Str();
	//	;
	//	if (name.find(":") != -1) {
	//		String replaced_name = name.replace(":", "_");
	//		Godot::print("Replacing " + name + " containing : with " + replaced_name);
	//		name = replaced_name;
	//	}
	//	return name;
	//}

	//const Transform _extract_ai_matrix_transform(const aiMatrix4x4 p_matrix) {
	//	aiQuaternion rotation;
	//	aiVector3t<ai_real> position;
	//	aiVector3t<ai_real> scaling;
	//	aiMatrix4x4 matrix = p_matrix;
	//	matrix.Decompose(scaling, rotation, position);
	//	Transform xform;
	//	xform.set_origin(_vec3_convert_to_godot(Vector3(position.x, position.y, position.z)));
	//	xform.basis = _set_quat_scale(Vector3(scaling.x, scaling.y, scaling.z), Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	//	return xform;
	//}

	//const Basis _set_quat_scale(Vector3 scaling, Quat rotation) {
	//	Basis basis;
	//	basis.set(scaling.x, 0.0f, 0.0f, 0.0f, scaling.y, 0.0f, 0.0f, 0.0f, scaling.z);
	//	Quat quat = _rot_convert_to_godot(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	//	quat.normalize();
	//	if (quat != Quat()) {
	//		Vector3 axis = Vector3();
	//		real_t angle = 0.0f;
	//		_get_axis_angle(Basis(quat), axis, angle);
	//		basis.rotate(axis, angle);
	//	}
	//	return basis;
	//}

	//const Vector3 _vec3_convert_to_godot(Vector3 p_pos) const {
	//	return p_pos;
	//}

	//const Quat _rot_convert_to_godot(Quat p_quat) const {
	//	return p_quat;
	//}
};