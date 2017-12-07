#include <core/Godot.hpp>
#include <Reference.hpp>

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>

#if defined( __unix__ ) || defined( __APPLE__ )

#include <sys/stat.h>

#define _stricmp strcasecmp
#endif

#include <cxxopts.hpp>

#include "FBX2glTF.h"
#include "utils/String_Utils.h"
#include "utils/File_Utils.h"
#include "Fbx2Raw.h"
#include "RawModel.h"
#include "Raw2Gltf.h"

bool verboseOutput = false;

using namespace godot;

class SimpleClass : public GodotScript<Reference> {
        GODOT_CLASS(SimpleClass);
public:
        SimpleClass() { }

        void test_void_method() {
                GltfOptions gltfOptions{
                        -1,            // keepAttribs
                        false,         // outputBinary
                        false,         // embedResources
                        false,         // useDraco
                        false,         // useKHRMatCom
                        false,         // usePBRMetRough
                        false,         // usePBRSpecGloss
                        false,         // useBlendShapeNormals
                        false,         // useBlendShapeTangents
                };
                Godot::print("This is test");
        }

        Variant method(Variant arg) {
            Variant ret;
            ret = arg;

            return ret;
        }

        static void _register_methods() {
           register_method("method", &SimpleClass::method);
       
       /**
        * How to register exports like gdscript
        * export var _name = "SimpleClass"
        **/
       register_property((char *)"base/name", &SimpleClass::_name, String("SimpleClass"));

           /** For registering signal **/
           // register_signal<SimpleClass>("signal_name");
        }
    
    String _name;
};

/** GDNative Initialize **/
GDNATIVE_INIT(godot_gdnative_init_options *options) {

}

/** GDNative Terminate **/
GDNATIVE_TERMINATE(godot_gdnative_terminate_options *options) {

}

/** NativeScript Initialize **/
NATIVESCRIPT_INIT() {
        register_class<SimpleClass>();
}