extends Control

# load the SIMPLE library
onready var data = preload("res://addons/ResourceImporterFBX/ResourceImporterFBX.gdn").new()

func _on_Button_pressed():
#  $Label.text = "Data = " + data.get_visible_name()
  data.import("res://cube.fbx", "res://main", {}, [], [])