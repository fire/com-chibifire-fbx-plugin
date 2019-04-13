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
	Spatial *root = Spatial::_new();

	Array skeletons;
	Array bone_names;

	Skeleton *s = Skeleton::_new();
	root->add_child(s);
	s->set_owner(root);
	skeletons.push_back(s);
	//_generate_node_bone(p_path, scene, scene->mRootNode, root, skeletons, bone_names, light_names, camera_names);

	RawNode node = raw.GetNode(raw.GetNodeById(raw.GetRootNode()));
	_generate_node(raw, node, root, root, skeletons, bone_names);

	//TODO(Ernest) Draco compression
	std::vector<RawModel> materialModels;
	raw.CreateMaterialModels(
			materialModels,
			true,
			true,
			true);

	for (const auto &surfaceModel : materialModels) {
		assert(surfaceModel.GetSurfaceCount() == 1);
		const RawSurface &rawSurface = surfaceModel.GetSurface(0);
		const long surfaceId = rawSurface.id;
		String name = rawSurface.name.c_str();
		Node *node = root->find_node(name);
		if (!node) {
			continue;
		}
		MeshInstance *mi = Object::cast_to<MeshInstance>(node);
		if (!mi) {
			continue;
		}
		SurfaceTool *st = SurfaceTool::_new();
		st->begin(Mesh::PRIMITIVE_TRIANGLES);

		if ((surfaceModel.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_POSITION) != 0) {
			const AttributeDefinition<Vec3f> ATTR_POSITION(
					"POSITION",
					&RawVertex::position,
					GLT_VEC3F,
					draco::GeometryAttribute::POSITION,
					draco::DT_FLOAT32);
			std::vector<Vec3f> attribArr;
			surfaceModel.GetAttributeArray<Vec3f>(attribArr, ATTR_POSITION.rawAttributeIx);

			for (auto a : attribArr) {
				st->add_vertex(Vector3(a.x, a.y, a.z));
			}
		}
		if ((surfaceModel.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_UV0) != 0) {
			const AttributeDefinition<Vec2f> ATTR_TEXCOORD_0(
					"TEXCOORD_0",
					&RawVertex::uv0,
					GLT_VEC2F,
					draco::GeometryAttribute::TEX_COORD,
					draco::DT_FLOAT32);
			std::vector<Vec2f> attribArrUV0;
			surfaceModel.GetAttributeArray<Vec2f>(attribArrUV0, ATTR_TEXCOORD_0.rawAttributeIx);

			for (auto a : attribArrUV0) {
				st->add_uv(Vector2(a.x, a.y));
			}
		}
		if ((surfaceModel.GetVertexAttributes() & RAW_VERTEX_ATTRIBUTE_UV1) != 0) {
			const AttributeDefinition<Vec2f> ATTR_TEXCOORD_1(
					"TEXCOORD_1",
					&RawVertex::uv1,
					GLT_VEC2F,
					draco::GeometryAttribute::TEX_COORD,
					draco::DT_FLOAT32);
			std::vector<Vec2f> attribArrUV1;
			surfaceModel.GetAttributeArray<Vec2f>(attribArrUV1, ATTR_TEXCOORD_1.rawAttributeIx);

			for (auto a : attribArrUV1) {
				st->add_uv2(Vector2(a.x, a.y));
			}
		}

		typedef uint32_t TriangleIndex;
		Array result;

		for (int i = 0; i < surfaceModel.GetTriangleCount(); i++) {
			st->add_index((TriangleIndex)surfaceModel.GetTriangle(i).verts[2]);
			st->add_index((TriangleIndex)surfaceModel.GetTriangle(i).verts[1]);
			st->add_index((TriangleIndex)surfaceModel.GetTriangle(i).verts[0]);
		}
		mi->set_mesh(st->commit());
	}

	return root;
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

void ComChibifireFbxImporter::_generate_node(const RawModel p_scene, const RawNode p_node, Node *p_parent, Node *p_owner, Array &p_skeletons, Array &r_bone_name) {
	Spatial *node = NULL;
	String node_name = p_node.name.c_str();
	Transform xform;
	float angle = 0.0f;
	Vec3f axis = Vec3f();
	xform.basis.rotate(Vector3(axis.x, axis.y, axis.z), angle);
	xform.basis.scale(Vector3(p_node.scale.x, p_node.scale.y, p_node.scale.z));
	p_node.rotation.ToAngleAxis(&angle, &axis);
	xform.origin = Vector3(p_node.translation.x, p_node.translation.y, p_node.translation.z);
	xform.orthonormalize();
	if (!p_node.isJoint) {
		node = Spatial::_new();
		node->set_name(node_name);
		p_parent->add_child(Object::cast_to<Node>(node));
		node->set_transform(xform);
		node->set_owner(p_owner);
		bool has_uvs = false;
	} else {
		node = Object::cast_to<Spatial>(p_parent);
	}

	if (p_node.surfaceId > 0) {
		for (size_t k = 0; k < p_skeletons.size(); k++) {
			Skeleton *s = Object::cast_to<Skeleton>(Object::___get_from_variant(p_skeletons[k]));
			node->get_parent()->remove_child(Object::cast_to<Node>(node));
			MeshInstance *mi = MeshInstance::_new();
			p_parent->remove_child(node);
			p_parent->add_child(mi);
			mi->set_owner(p_owner);
			mi->set_name(node_name);
			mi->set_transform(xform);
			mi->set_skeleton_path(mi->get_path_to(s));
			p_skeletons[k] = s;
		}
	}

	for (size_t i = 0; i < p_node.childIds.size(); i++) {
		_generate_node(p_scene, p_scene.GetNode(p_scene.GetNodeById(p_node.childIds[i])), node, p_owner, p_skeletons, r_bone_name);
	}
}
