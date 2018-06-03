#include "ComChibifireFbxImporter.h"
#include <Godot.hpp>

using namespace godot;

extern "C" void godot_gdnative_init(godot_gdnative_init_options *options) {
	Godot::gdnative_init(options);
}

extern "C" void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
	Godot::gdnative_terminate(options);
}

extern "C" void godot_nativescript_init(void *handle) {
	Godot::nativescript_init(handle);
	register_tool_class<ComChibifireFbxImporter>();
}