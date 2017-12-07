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

#include "EditorImportPlugin.hpp"

using godot::GodotScript;
using godot::register_class;
using godot::Reference;
using godot::Variant;
using godot::EditorImportPlugin;
using godot::String;
using godot::Reference;
using godot::Array;
using godot::Dictionary;
using godot::Ref;
using godot::ArrayMesh;

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

class ResourceImporterFBX : public GodotScript<EditorImportPlugin> {
private:
  Ref<ArrayMesh> import_fbx(const String source_file, const String save_path, const Dictionary options, const Array r_platform_variants, const Array r_gen_files);
  GODOT_CLASS(ResourceImporterFBX);

public:
  ResourceImporterFBX() {}

  String get_importer_name() const;
  String get_visible_name() const;
  int get_preset_count() const;
  String get_preset_name(const int preset) const;
  Array get_recognized_extensions() const;
  Array get_import_options(const int preset) const;
  String get_save_extension() const;
  String get_resource_type() const;
  bool get_option_visibility(const String option, const Dictionary options) const;
  int import(const String source_file, const String save_path, const Dictionary options, const Array r_platform_variants, const Array r_gen_files);

  static void _register_methods();
};