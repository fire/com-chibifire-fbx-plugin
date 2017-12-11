tool
extends EditorPlugin

var import_plugin = null

func get_name():
    return "Fbx Importer"

func _enter_tree():
    import_plugin = load("res://addons/ComChibifireFbxImporter/ComChibifireFbxImporter.gdns").new()
    add_scene_import_plugin(import_plugin)

func _exit_tree():
    remove_scene_import_plugin(import_plugin)
    import_plugin = null