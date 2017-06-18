#pragma once

#include <godot.h>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>
#include <Reference.hpp>
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

class ResourceImporterFBX : public GodotScript<EditorImportPlugin> {
	GODOT_CLASS(ResourceImporterFBX);
public:
	ResourceImporterFBX() { }
	
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

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_class<ResourceImporterFBX>();
}
