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

	const String fbx_file = ProjectSettings::globalize_path(path);
	const String path_dir_global = ProjectSettings::globalize_path(path.get_base_dir());

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
	const String gltf_global = ProjectSettings::globalize_path(gltf_path);

	outStream.open(gltf_global.alloc_c_string(), std::ios::trunc | std::ios::ate | std::ios::out | std::ios::binary);
	if (outStream.fail()) {
		Godot::print(godot::String("ERROR:: Couldn't open file for writing: ") + gltf_global);
		return nullptr;
	}

	const ModelData *data_render_model = Raw2Gltf(outStream, path_dir_global.alloc_c_string(), raw, gltfOptions);

	outStream.flush();
	outStream.close();

	printf(
			"Wrote %lu bytes of glTF to %s.\n",
			(unsigned long)(outStream.tellp() - streamStart), gltf_global.alloc_c_string());

	delete data_render_model;

	return owner->import_scene_from_other_importer(gltf_path, flags, bake_fps);
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
