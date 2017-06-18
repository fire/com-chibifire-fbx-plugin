#pragma once

#include "ResourceImporter.hpp"

using godot::register_class;
using godot::GodotScript;
using godot::Reference;
using godot::Variant;
using godot::ResourceImporter;

class ResourceImporterFBX : public GodotScript<Reference> {
	GODOT_CLASS(ResourceImporterFBX);
public:
	ResourceImporterFBX() { }

	void test_void_method();

	Variant method(Variant arg);

	static void _register_methods();
};

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_class<ResourceImporterFBX>();
}