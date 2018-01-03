tool
extends EditorPlugin

var import_plugin = null

func get_name():
    return "Fbx Importer"

func _enter_tree():
    import_plugin = load("res://addons/com_chibifire_fbx_importer/com_chibifire_fbx_importer.gdns").new()
    add_scene_import_plugin(import_plugin)

func _exit_tree():
    remove_scene_import_plugin(import_plugin)
    import_plugin = null