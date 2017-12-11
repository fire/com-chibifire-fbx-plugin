tool
extends EditorPlugin

var import_plugin = null

func get_name():
    return "Fbx Importer"

func _enter_tree():
    import_plugin = load("res://addons/ComChibifireFbxImporter/ComChibifireFbxImporter.gdns").new()
    add_scene_import_plugin(import_plugin)
    get_editor_interface().get_resource_filesystem().call_deferred("scan")	

func _exit_tree():
    remove_scene_import_plugin(import_plugin)
    get_editor_interface().get_resource_filesystem().call_deferred("scan")	
    import_plugin = null