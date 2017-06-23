tool extends EditorPlugin

var import_plugin

func _enter_tree():
  import_plugin = preload("res://addons/ResourceImporterFBX/ResourceImporterFBXPlugin.gd").new()
  add_import_plugin(import_plugin)

func _exit_tree():
  remove_import_plugin3(import_plugin)
