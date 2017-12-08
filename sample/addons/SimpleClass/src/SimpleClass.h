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

#include <core/Godot.hpp>
#include <Reference.hpp>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>
#include <ArrayMesh.hpp>
#include <Ref.hpp>
#include <platform_config.h>
#include <core/safe_refcount.h>
#include <core/list.h>
#include <Node.hpp>
#include <String.hpp>

#include <EditorSceneImporter.hpp>

using godot::GodotScript;
using godot::register_class;
using godot::Reference;
using godot::Variant;
using godot::EditorSceneImporter;
using godot::String;
using godot::Reference;
using godot::Array;
using godot::Dictionary;
using godot::Ref;
using godot::ArrayMesh;
using godot::Node;

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

class SimpleClass : public GodotScript<EditorSceneImporter> {
private:
  GODOT_CLASS(SimpleClass);

public:
  SimpleClass() {}

  void get_extensions(List<String> *r_extensions) const;
  uint32_t get_import_flags() const;
  Node *import_scene(const String &p_path, uint32_t p_flags, int p_bake_fps, List<String> *r_missing_deps, godot::Error *r_err);
  static void _register_methods();
};