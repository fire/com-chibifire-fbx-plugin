tool
extends EditorImportPlugin

var fbx_lib 

func get_recognized_extensions():
  return ["fbx"]

func import(source_file, save_path, options, r_platform_variants, r_gen_files): 
  fbx_lib = preload("res://addons/ResourceImporterFBX/ResourceImporterFBX.gdn")
  fbx_lib.new().import(path, path, {}, [], [])
  return "OK"

func get_import_options ( preset ):
  return []

func  get_importer_name ( ):
  return "fbx_mesh"

func get_option_visibility (  option,  options ):
  return true

func get_preset_count ( ):
  return 0

func get_preset_name ( preset ):
  return ""

func get_resource_type ( ):
  return "ArrayMesh"

func get_save_extension ( ):
  return "mesh"

func get_visible_name ( ):
  return "FBX As Mesh"