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
	
	String get_importer_name();
	String get_visible_name();
	int get_preset_count();
	String get_preset_name(const int preset);
	Array get_recognized_extensions();
	Array get_import_options(const int preset);
	String get_save_extension();
	String get_resource_type();
	bool get_option_visibility(const String option, const Dictionary options);
	int import(const String source_file, const String save_path, const Dictionary options, const Array r_platform_variants, const Array r_gen_files);

	static void _register_methods();
};

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_class<ResourceImporterFBX>();
}
