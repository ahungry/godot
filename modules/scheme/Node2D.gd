extends Node2D

var s = Scheme.new()

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
func scheme():
	#var s = Scheme.new()
	#s.add(10)
	#s.add(20)
	#print(s.get_total())
	#s.reset()
	printt(s.listen(3.3))
	printt(s.eval("(+ 1 2 3)"))
	printt(s.processInput(InputHandler.new()))

# Called when the node enters the scene tree for the first time.
func _ready():
	printt ("Eeeeey")
	scheme()

func _process(delta):
	pass
	# Incremented via scheme
	# printt(s.get_total())
	# $Label.text = str(s.get_total())

class InputHandler:
	func getInputs():
		printt("I was called from somewhere!")
		return 1

# var game = Game.new()
# game.processInput(InputHandler.new())
