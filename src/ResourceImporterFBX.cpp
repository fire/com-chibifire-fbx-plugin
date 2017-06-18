#include <godot.h>

#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

#include <ResourceSaver.hpp>
#include <Mesh.hpp>
#include <SurfaceTool.hpp>

#include <Reference.hpp>

#include "ResourceImporterFBX.h"

using namespace godot;

inline void ResourceImporterFBX::test_void_method() {
	Godot::print("This is test");
}

inline Variant ResourceImporterFBX::method(Variant arg) {
	Variant ret;
	ret = arg;

	return ret;
}

inline void ResourceImporterFBX::_register_methods() {
	register_method("method", &ResourceImporterFBX::method);
}
