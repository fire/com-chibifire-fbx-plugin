tool extends EditorPlugin

var plugin

func _enter_tree():
	plugin = load("res://addons/SimpleClass/SimpleClass.gdns").new()
	add_scene_import_plugin(plugin)
	print("Simple Class enabled")
	var simple = load("res://addons/SimpleClass/SimpleClass.gdns").new()
	simple._import_scene("res://miniHouse_FBX.FBX", 0, 60)

func _exit_tree():
	pass