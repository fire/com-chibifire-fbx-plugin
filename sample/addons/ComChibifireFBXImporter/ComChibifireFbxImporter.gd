tool
extends EditorPlugin

var plugin

func _enter_tree():
    plugin = load("res://addons/ComChibifireFbxImporter/ComChibifireFbxImporter.gdns").new()
    add_scene_import_plugin(plugin)
    
func _exit_tree():
    remove_scene_import_plugin(plugin)
    plugin = null