extends Control

# load the SIMPLE library
onready var data = preload("res://ResourceImporterFBX.gdn").new()

func _on_Button_pressed():
  $Label.text = "Data = " + data.get_visible_name()
  data.import("res://cube.fbx", "res://main", {}, [], [])
#   var a = []
#   a.resize(9)
#   var test_tris = []
#   test_tris.push_back(Vector3(0, 1, 0))
#   test_tris.push_back(Vector3(1, 0, 0))
#   test_tris.push_back(Vector3(0, 0, 1))
#
#   var test_indices = []
#   test_indices.push_back(0)
#   test_indices.push_back(1)
#   test_indices.push_back(2)
#
#   a[0] = test_tris
#   a[8] = test_indices
#   var array_mesh = ArrayMesh.new() 
#   array_mesh.add_surface_from_arrays(4, a)
#
#   return ResourceSaver.save("res://main.mesh", array_mesh)
#  