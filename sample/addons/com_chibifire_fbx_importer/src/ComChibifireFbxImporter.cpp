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

#include <Godot.hpp>
#include <PoolArrays.hpp>
#include <ProjectSettings.hpp>
#include <Skeleton.hpp>

#ifndef CLAMP
#define CLAMP(m_a, m_min, m_max) (((m_a) < (m_min)) ? (m_min) : (((m_a) > (m_max)) ? m_max : m_a))
#endif

using namespace godot;

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

Node *ComChibifireFbxImporter::import_scene(const String path, const int64_t flags, const int64_t bake_fps) {
	GltfOptions gltfOptions;
	gltfOptions.keepAttribs = -1;
	gltfOptions.outputBinary = true;
	gltfOptions.embedResources = true;
	gltfOptions.draco.enabled = false;
	gltfOptions.draco.compressionLevel = -1;
	gltfOptions.draco.quantBitsPosition = -1;
	gltfOptions.draco.quantBitsTexCoord = -1;
	gltfOptions.draco.quantBitsNormal = -1;
	gltfOptions.draco.quantBitsColor = -1;
	gltfOptions.draco.quantBitsGeneric = -1;
	gltfOptions.useKHRMatUnlit = false;
	gltfOptions.usePBRMetRough = true;
	gltfOptions.useBlendShapeNormals = false;
	gltfOptions.useBlendShapeTangents = false;
	gltfOptions.computeNormals = ComputeNormalsOption::MISSING;
	// compute vertex normals from geometry.
	gltfOptions.useLongIndices = UseLongIndicesOptions::AUTO; // When to use 32-bit indices.

	RawModel raw;

	const String fbx_file = ProjectSettings::get_singleton()->globalize_path(path);
	const String path_dir_global = ProjectSettings::get_singleton()->globalize_path(path.get_base_dir());

	if (!LoadFBXFile(raw, fbx_file.alloc_c_string(), godot::String("png;jpg;jpeg").alloc_c_string())) {
		return nullptr;
	}

	std::vector<std::function<Vec2f(Vec2f)> > texturesTransforms;
	texturesTransforms.emplace_back([](Vec2f uv) { return Vec2f(uv[0], 1.0f - uv[1]); });

	if (!texturesTransforms.empty()) {
		raw.TransformTextures(texturesTransforms);
	}

	raw.Condense();
	raw.TransformGeometry(gltfOptions.computeNormals);

	std::ofstream outStream; // note: auto-flushes in destructor
	const auto streamStart = outStream.tellp();

	const String file = path.get_basename().get_file() + "-" + path.md5_text() + String(".glb");
	const String gltf_path = String("res://.import/").plus_file(file);
	const String gltf_global = ProjectSettings::get_singleton()->globalize_path(gltf_path);

	outStream.open(gltf_global.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
	if (outStream.fail()) {
		Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + gltf_global);
		return nullptr;
	}

	Spatial *root = Spatial::_new();
	//For all cameras
	//ERR_FAIL_INDEX(i, state.cameras.size());
	//Camera *camera = memnew(Camera);

	//if (c.perspective) {
	//	camera->set_perspective(c.fov_size, c.znear, c.znear);
	//} else {
	//	camera->set_orthogonal(c.fov_size, c.znear, c.znear);
	//}

	// For all lights
	//Light *light = memnew(Light);
	//ERR_FAIL_INDEX(i, lights.size());

	//Vector3 scale = Vector3(100.0f, 100.0f, 100.0f);
	//if (scene == NULL) {
	//	return NULL;
	//}
	//if (scene->mMetaData != NULL) {
	//	float unit_scale_factor = 1.0f;
	//	scene->mMetaData->Get("UnitScaleFactor", unit_scale_factor);
	//	scale = Vector3(unit_scale_factor, unit_scale_factor, unit_scale_factor) * scale;
	//}
	//scale = Vector3(1.0f, 1.0f, 1.0f) / scale;
	//if (p_path.get_extension() == String("fbx")) {
	//	//root->set_rotation_degrees(Vector3(-90.0f, 0.f, 0.0f));
	//	root->set_scale(scale);
	//}
	Array skeletons;
	Array bone_names;
	//Array light_names;
	//Array camera_names;
	//for (size_t l = 0; l < scene->mNumLights; l++) {
	//	light_names.push_back(_ai_string_to_string(scene->mLights[l]->mName));
	//}
	//for (size_t c = 0; c < scene->mNumCameras; c++) {
	//	camera_names.push_back(_ai_string_to_string(scene->mCameras[c]->mName));
	//}

	Skeleton *s = Skeleton::_new();
	root->add_child(s);
	s->set_owner(root);
	skeletons.push_back(s);
	//_generate_node_bone(p_path, scene, scene->mRootNode, root, skeletons, bone_names, light_names, camera_names);

	RawNode &node = raw.GetNode(raw.GetNodeById(raw.GetRootNode()));
	_generate_node(raw, node, root, root, skeletons, bone_names);
	//if (skeletons.size() == 0) {
	//	return NULL;
	//}

	//for (size_t j = 0; j < skeletons.size(); j++) {
	//	Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(skeletons[j]));
	//	s->set_transform(_get_armature_xform(scene, s, bone_names, root, s).affine_inverse());
	//	skeletons[j] = s;
	//}

	//for (size_t i = 0; i < skeletons.size(); i++) {
	//	Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(skeletons[i]));
	//	for (size_t j = 0; j < s->get_bone_count(); j++) {
	//		String bone_name = s->get_bone_name(j);
	//		int32_t node_parent_index = -1;
	//		const aiNode *bone_node = scene->mRootNode->FindNode(_string_to_ai_string(bone_name));
	//		if (bone_node != NULL && bone_node->mParent != NULL) {
	//			node_parent_index = s->find_bone(_ai_string_to_string(bone_node->mParent->mName));
	//		}
	//		s->set_bone_parent(j, node_parent_index);
	//	}
	//	for (int i = s->get_bone_count() - 1; i >= 0; i--) {
	//		if (s->get_bone_parent(i) >= 0) {
	//			s->set_bone_rest(i, s->get_bone_rest(s->get_bone_parent(i)).affine_inverse() * s->get_bone_rest(i));
	//		}
	//	}
	//}

	//const bool is_clear_bones = false;
	//if (is_clear_bones) {
	//	for (size_t i = 0; i < scene->mNumMeshes; i++) {
	//		for (size_t j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
	//			Node *node = root->find_node(_ai_string_to_string(scene->mMeshes[i]->mBones[j]->mName));
	//			if (node != NULL) {
	//				node->get_parent()->remove_child(node);
	//			}
	//		}
	//	}
	//}

	//AnimationPlayer *ap = AnimationPlayer::_new();
	//root->add_child(ap);
	//ap->set_owner(root);
	//ap->set_name("AnimationPlayer");

	//for (int i = 0; i < scene->mNumAnimations; i++) {
	//	const aiScene *p_scene = scene;
	//	int32_t p_index = i;
	//	String name = "Animation";
	//	aiAnimation const *anim = NULL;
	//	if (p_index != -1) {
	//		anim = p_scene->mAnimations[p_index];
	//		if (anim->mName.length > 0) {
	//			name = _ai_string_to_string(anim->mName);
	//		}
	//	}

	//	Animation *animation = Animation::_new();
	//	float ticks_per_second = 25.0f;
	//	float length = 0.0f;
	//	if (p_index != -1) {
	//		animation->set_name(name);
	//		ticks_per_second = p_scene->mAnimations[p_index]->mTicksPerSecond != 0 ?
	//								   p_scene->mAnimations[0]->mTicksPerSecond :
	//								   25.0f;
	//		length = anim->mDuration / ticks_per_second;
	//	}
	//if (anim) {
	//	for (size_t i = 0; i < anim->mNumChannels; i++) {
	//		if (skeletons.size()) {
	//			for (size_t si = 0; si < skeletons.size(); si++) {
	//				const aiNodeAnim *track = anim->mChannels[i];
	//				String path;
	//				String node_name = _ai_string_to_string(track->mNodeName);
	//				Skeleton *sk = Object::cast_to<Skeleton>(Object::___get_from_variant(skeletons[si]));
	//				//need to find the path
	//				NodePath node_path = node_name;
	//				bool is_bone_found = false;
	//				Array bone_names;
	//				for (size_t k = 0; k < sk->get_bone_count(); k++) {
	//					if (sk->get_bone_name(k).begins_with(node_name)) {
	//						node_name = sk->get_bone_name(k);
	//						break;
	//					}
	//				}
	//				if (sk->find_bone(node_name) != -1) {
	//					is_bone_found = true;
	//				}
	//				if (is_bone_found) {
	//					path = ap->get_owner()->get_path_to(sk);
	//					if (path == String()) {
	//						Godot::print("Can't find bone to animate");
	//						continue;
	//					}
	//					node_path = NodePath(path + String(":") + node_name);
	//				} else {
	//					Node *node = ap->get_owner()->find_node(node_name);
	//					if (node == NULL) {
	//						continue;
	//					}
	//					path = ap->get_owner()->get_path_to(node);
	//					if (path == String()) {
	//						continue;
	//					}
	//					node_path = NodePath(path);
	//				}

	//				if (track->mNumRotationKeys || track->mNumPositionKeys || track->mNumScalingKeys) {
	//					//make transform track
	//					int track_idx = animation->get_track_count();
	//					animation->add_track(Animation::TYPE_TRANSFORM);
	//					animation->track_set_path(track_idx, node_path);
	//					//first determine animation length

	//					float increment = 1.0 / float(p_bake_fps);
	//					float time = 0.0;

	//					Vector3 base_pos;
	//					Quat base_rot;
	//					Vector3 base_scale = Vector3(1, 1, 1);

	//					if (track->mNumRotationKeys) {
	//						aiQuatKey key = track->mRotationKeys[0];
	//						real_t x = key.mValue.x;
	//						real_t y = key.mValue.y;
	//						real_t z = key.mValue.z;
	//						real_t w = key.mValue.w;
	//						Quat q(x, y, z, w);
	//						base_rot = _rot_convert_to_godot(q.normalized());
	//					}

	//					if (track->mNumPositionKeys) {
	//						aiVectorKey key = track->mPositionKeys[0];
	//						real_t x = key.mValue.x;
	//						real_t y = key.mValue.y;
	//						real_t z = key.mValue.z;
	//						base_pos = _vec3_convert_to_godot(Vector3(x, y, z));
	//					}

	//					if (track->mNumScalingKeys) {
	//						aiVectorKey key = track->mScalingKeys[0];
	//						real_t x = key.mValue.x;
	//						real_t y = key.mValue.y;
	//						real_t z = key.mValue.z;
	//						base_scale = _vec3_convert_to_godot(Vector3(x, y, z));
	//					}

	//					bool last = false;

	//					Array pos_values;
	//					Array pos_times;
	//					Array scale_values;
	//					Array scale_times;
	//					Array rot_values;
	//					Array rot_times;

	//					for (size_t p = 0; p < track->mNumPositionKeys; p++) {
	//						aiVector3D pos = track->mPositionKeys[p].mValue;
	//						pos_values.push_back(_vec3_convert_to_godot(Vector3(pos.x, pos.y, pos.z)));
	//						pos_times.push_back(track->mPositionKeys[p].mTime / ticks_per_second);
	//					}

	//					for (size_t r = 0; r < track->mNumRotationKeys; r++) {
	//						aiQuaternion quat = track->mRotationKeys[r].mValue;
	//						rot_values.push_back(_rot_convert_to_godot(Quat(quat.x, quat.y, quat.z, quat.w)));
	//						rot_times.push_back(track->mRotationKeys[r].mTime / ticks_per_second);
	//					}

	//					for (size_t sc = 0; sc < track->mNumScalingKeys; sc++) {
	//						aiVector3D scale = track->mScalingKeys[sc].mValue;
	//						scale_values.push_back(_vec3_convert_to_godot(Vector3(scale.x, scale.y, scale.z)));
	//						scale_times.push_back(track->mScalingKeys[sc].mTime / ticks_per_second);
	//					}
	//					while (true) {
	//						Vector3 pos = base_pos;
	//						Quat rot = base_rot;
	//						Vector3 scale = base_scale;

	//						if (pos_values.size()) {
	//							pos = _interpolate_track_vector3(pos_times, pos_values, time, AssetImportAnimation::INTERP_LINEAR);
	//						}

	//						if (rot_values.size()) {
	//							rot = _interpolate_track_quat(rot_times, rot_values, time, AssetImportAnimation::INTERP_LINEAR);
	//						}

	//						if (scale_values.size()) {
	//							scale = _interpolate_track_vector3(scale_times, scale_values, time, AssetImportAnimation::INTERP_LINEAR);
	//						}

	//						if (sk != NULL && sk->find_bone(node_name) != -1) {
	//							Transform xform;
	//							//xform.basis = Basis(rot);
	//							//xform.basis.scale(scale);
	//							xform.basis = _set_quat_scale(scale, rot);
	//							xform.origin = pos;

	//							int bone = sk->find_bone(node_name);
	//							xform = sk->get_bone_rest(bone).affine_inverse() * xform;
	//							Vector3 axis = Vector3();
	//							real_t angle = 0.0f;
	//							xform.basis = xform.basis.orthonormalized();
	//							_get_axis_angle(xform.basis, axis, angle);
	//							rot = Quat(axis, angle);
	//							scale = xform.basis.get_scale();
	//							pos = xform.origin;
	//						}
	//						animation->transform_track_insert_key(track_idx, time, pos, rot, scale);

	//						if (last) {
	//							break;
	//						}
	//						time += increment;
	//						if (time >= length) {
	//							last = true;
	//							time = length;
	//						}
	//					}
	//				}
	//			}
	//		} else {
	//			const aiNodeAnim *track = anim->mChannels[i];
	//			String path;
	//			String node_name = _ai_string_to_string(track->mNodeName);
	//			//need to find the path
	//			NodePath node_path = node_name;

	//			Node *node = ap->get_owner()->find_node(node_name);
	//			if (node == NULL) {
	//				continue;
	//			}
	//			path = ap->get_owner()->get_path_to(node);
	//			if (path == String()) {
	//				continue;
	//			}
	//			node_path = NodePath(path);

	//			if (track->mNumRotationKeys || track->mNumPositionKeys || track->mNumScalingKeys) {
	//				//make transform track
	//				int track_idx = animation->get_track_count();
	//				animation->add_track(Animation::TYPE_TRANSFORM);
	//				animation->track_set_path(track_idx, node_path);
	//				//first determine animation length

	//				float increment = 1.0 / float(p_bake_fps);
	//				float time = 0.0;

	//				Vector3 base_pos;
	//				Quat base_rot;
	//				Vector3 base_scale = Vector3(1, 1, 1);

	//				if (track->mNumRotationKeys) {
	//					aiQuatKey key = track->mRotationKeys[i];
	//					real_t x = key.mValue.x;
	//					real_t y = key.mValue.y;
	//					real_t z = key.mValue.z;
	//					real_t w = key.mValue.w;
	//					Quat q(x, y, z, w);
	//					base_rot = _rot_convert_to_godot(q);
	//				}

	//				if (track->mNumPositionKeys) {
	//					aiVectorKey key = track->mPositionKeys[i];
	//					real_t x = key.mValue.x;
	//					real_t y = key.mValue.y;
	//					real_t z = key.mValue.z;
	//					base_pos = _vec3_convert_to_godot(Vector3(x, y, z));
	//				}

	//				if (track->mNumScalingKeys) {
	//					aiVectorKey key = track->mScalingKeys[i];
	//					real_t x = key.mValue.x;
	//					real_t y = key.mValue.y;
	//					real_t z = key.mValue.z;
	//					base_scale = _vec3_convert_to_godot(Vector3(x, y, z));
	//				}

	//				bool last = false;

	//				Array pos_values;
	//				Array pos_times;
	//				Array scale_values;
	//				Array scale_times;
	//				Array rot_values;
	//				Array rot_times;

	//				for (size_t p = 0; p < track->mNumPositionKeys; p++) {
	//					aiVector3D pos = track->mPositionKeys[p].mValue;
	//					pos_values.push_back(_vec3_convert_to_godot(Vector3(pos.x, pos.y, pos.z)));
	//					pos_times.push_back(track->mPositionKeys[p].mTime / ticks_per_second);
	//				}

	//				for (size_t r = 0; r < track->mNumRotationKeys; r++) {
	//					aiQuaternion quat = track->mRotationKeys[r].mValue;
	//					rot_values.push_back(_rot_convert_to_godot(Quat(quat.x, quat.y, quat.z, quat.w)));
	//					rot_times.push_back(track->mRotationKeys[r].mTime / ticks_per_second);
	//				}

	//				for (size_t sc = 0; sc < track->mNumScalingKeys; sc++) {
	//					aiVector3D scale = track->mScalingKeys[sc].mValue;
	//					scale_values.push_back(_vec3_convert_to_godot(Vector3(scale.x, scale.y, scale.z)));
	//					scale_times.push_back(track->mScalingKeys[sc].mTime / ticks_per_second);
	//				}
	//				while (true) {
	//					Vector3 pos = base_pos;
	//					Quat rot = base_rot;
	//					Vector3 scale = base_scale;

	//					if (pos_values.size()) {
	//						pos = _interpolate_track_vector3(pos_times, pos_values, time, AssetImportAnimation::INTERP_LINEAR);
	//					}

	//					if (rot_values.size()) {
	//						rot = _interpolate_track_quat(rot_times, rot_values, time, AssetImportAnimation::INTERP_LINEAR);
	//					}

	//					if (scale_values.size()) {
	//						scale = _interpolate_track_vector3(scale_times, scale_values, time, AssetImportAnimation::INTERP_LINEAR);
	//					}

	//					animation->transform_track_insert_key(track_idx, time, pos, rot, scale);

	//					if (last) {
	//						break;
	//					}
	//					time += increment;
	//					if (time >= length) {
	//						last = true;
	//						time = length;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//	if (false) {
	//		for (int i = 0; i < anim->mNumMeshChannels; i++) {
	//			const aiMeshAnim *anim_mesh = anim->mMeshChannels[i];
	//			String prop_name = _ai_string_to_string(anim_mesh->mName);
	//			String prop = "blend_shapes/" + String(prop_name);
	//			String path = ap->get_owner()->get_path_to(ap->get_owner()->find_node(prop_name));
	//			if (path == String()) {
	//				Godot::print("Can't find mesh in scene");
	//				continue;
	//			}
	//			String node_path = String(node_path) + ":" + prop;

	//			int track_idx = animation->get_track_count();
	//			animation->add_track(Animation::TYPE_VALUE);
	//			animation->track_set_path(track_idx, NodePath(node_path));

	//			//must bake, apologies.
	//			float increment = 1.0 / float(p_bake_fps);
	//			float time = 0.0;

	//			bool last = false;

	//			Array values;
	//			Array times;
	//			for (size_t i = 0; i < anim_mesh->mNumKeys; i++) {
	//				uint32_t value = anim_mesh->mKeys[i].mValue;
	//				//mAnimMeshes[*value];
	//				// TODO Add vertex animation shapes
	//				values.push_back(value);
	//				times.push_back(anim_mesh->mKeys[i].mTime);
	//			}

	//			while (true) {
	//				_interpolate_track_real(times, values, time, AssetImportAnimation::INTERP_LINEAR);
	//				if (last) {
	//					break;
	//				}

	//				time += increment;
	//				if (time >= length) {
	//					last = true;
	//					time = length;
	//				}
	//			}
	//		}
	//		for (int i = 0; i < anim->mNumMorphMeshChannels; i++) {
	//			const aiMeshMorphAnim *morph_mesh = anim->mMorphMeshChannels[i];
	//			String prop_name = _ai_string_to_string(morph_mesh->mName);
	//			String prop = String(prop_name);
	//			String path = ap->get_owner()->get_path_to(ap->get_owner()->find_node(prop_name));
	//			if (path == String()) {
	//				Godot::print("Can't find mesh in scene");
	//				continue;
	//			}
	//			String node_path = String(node_path) + ":" + prop;

	//			int track_idx = animation->get_track_count();
	//			animation->add_track(Animation::TYPE_VALUE);
	//			animation->track_set_path(track_idx, NodePath(node_path));

	//			//must bake, apologies.
	//			float increment = 1.0 / float(p_bake_fps);
	//			float time = 0.0;

	//			bool last = false;

	//			Array values;
	//			Array times;
	//			for (size_t i = 0; i < morph_mesh->mNumKeys; i++) {
	//				uint32_t *value = morph_mesh->mKeys[i].mValues;
	//				// TODO Add blend shapes
	//				values.push_back(*value);
	//				times.push_back(morph_mesh->mKeys[i].mTime);
	//			}
	//			while (true) {
	//				_interpolate_track_real(times, values, time, AssetImportAnimation::INTERP_LINEAR);
	//				if (last) {
	//					break;
	//				}
	//				time += increment;
	//				if (time >= length) {
	//					last = true;
	//					time = length;
	//				}
	//			}
	//		}
	//	}
	//	animation->set_length(length);
	//	if (animation->get_track_count()) {
	//		ap->add_animation(name, animation);
	//	}
	//}
	//Array animation_names = ap->get_animation_list();
	//if (animation_names.size() == 0) {
	//	root->remove_child(ap);
	//}

	return root;

	//const ModelData *data_render_model = Raw2Gltf(outStream, path_dir_global.alloc_c_string(), raw, gltfOptions);

	//outStream.flush();
	//outStream.close();

	//printf(
	//		"Wrote %lu bytes of glTF to %s.\n",
	//		(unsigned long)(outStream.tellp() - streamStart), gltf_global.alloc_c_string());

	//delete data_render_model;
	//return import_scene_from_other_importer(gltf_path, flags, bake_fps);
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

void ComChibifireFbxImporter::_generate_node(const RawModel &p_scene, const RawNode &p_node, Node *p_parent, Node *p_owner, Array &p_skeletons, Array &r_bone_name) {
	Spatial *node = NULL;
	if (!p_node.isJoint) {
		node = Spatial::_new();
		String node_name = p_node.name.c_str();
		node->set_name(node_name);
		p_parent->add_child(Object::cast_to<Node>(node));
		Transform xform;
		float angle = 0.0f;
		Vec3f axis = Vec3f();
		xform.basis.rotate(Vector3(axis.x, axis.y, axis.z), angle);
		xform.basis.scale(Vector3(p_node.scale.x, p_node.scale.y, p_node.scale.z));
		p_node.rotation.ToAngleAxis(&angle, &axis);
		xform.origin = Vector3(p_node.translation.x, p_node.translation.y, p_node.translation.z);
		node->set_transform(xform);
		node->set_owner(p_owner);
		bool has_uvs = false;
	} else {
		node = Object::cast_to<Spatial>(p_parent);
	}


	//if (p_node.surfaceId > 0) {
	//	for (size_t k = 0; k < p_skeletons.size(); k++) {
	//		Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(p_skeletons[k]));
	//		node->get_parent()->remove_child(Object::cast_to<Node>(node));
	//		MeshInstance *mi = MeshInstance::_new();
	//		node = mi;
	//		p_parent->add_child(node);
	//		node->set_owner(p_owner);
	//		node->set_name(node_name);
	//		int surfaceIndex = p_scene.GetSurfaceById(p_node.surfaceId);
	//		const RawSurface &rawSurface = p_scene.GetSurface(surfaceIndex);
	//		node->set_transform(xform);
	//		mi->set_skeleton_path(mi->get_path_to(s));

	//		//_add_mesh_to_mesh_instance(p_node, p_scene, has_uvs, s, mi, p_owner, r_bone_name);
	//		p_skeletons[k] = s;
	//	}
	//}
	for (size_t i = 0; i < p_node.childIds.size(); i++) {
		_generate_node(p_scene, p_scene.GetNode(p_scene.GetNodeById(p_node.childIds[i])), node, p_owner, p_skeletons, r_bone_name);
	}
}
