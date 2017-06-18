#include <godot.h>

#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

#include <ResourceSaver.hpp>
#include <Mesh.hpp>
#include <SurfaceTool.hpp>

#include <Reference.hpp>

using namespace godot;

class ResourceImporterFBX : public GodotScript<Reference> {
	GODOT_CLASS(ResourceImporterFBX);
public:
	ResourceImporterFBX() { }

	void test_void_method() {
		Godot::print("This is test");
	}

	Variant method(Variant arg) {
		Variant ret;
		ret = arg;

		return ret;
	}

	static void _register_methods() {
		register_method("method", &ResourceImporterFBX::method);
	}
};

GODOT_NATIVE_INIT(godot_native_init_options *options) {
	register_class<ResourceImporterFBX>();
}