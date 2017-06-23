tool extends EditorPlugin

onready var import_plugin = preload("res://addons/ResourceImporterFBX/ResourceImporterFBXPlugin.gd").new()

func _enter_tree():
  add_import_plugin(import_plugin)

func _exit_tree():
  remove_import_plugin3(import_plugin)
