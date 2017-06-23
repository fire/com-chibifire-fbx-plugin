extends Control

onready var fbx = preload("res://addons/ResourceImporterFBX/ResourceImporterFBX.gdn").new()

func _on_Button_pressed():
  $Label.text = str(fbx.get_recognized_extensions())