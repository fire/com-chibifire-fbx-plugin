extends Control

func _on_Button_pressed():
  var simple = load("res://addons/SimpleClass/SimpleClass.gdns").new()
  print(simple.method("Test argument"));
