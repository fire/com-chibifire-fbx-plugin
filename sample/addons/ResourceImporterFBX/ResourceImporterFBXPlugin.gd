tool
extends EditorImportPlugin

onready var fbx_lib = preload("res://addons/ResourceImporterFBX/ResourceImporterFBX.gdn")

func get_recognized_extensions():
  return ["fbx"]

func import(path, metadata):
  fbx_lib.new().import("res://cube.fbx", "res://cube.fbx" + "mesh", {}, [], [])
  return 0

func get_import_options ( preset ):
  return []

func  get_importer_name ( ):
  return ""

func get_option_visibility (  option,  options ):
  return true

func get_preset_count ( ):
  return 0

func get_preset_name ( preset ):
  return ""

func get_resource_type ( ):
  return ""

func get_save_extension ( ):
  return "mesh"

func get_visible_name ( ):
  return ""