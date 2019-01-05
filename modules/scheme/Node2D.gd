extends Node2D

var xxx = InputHandler.new()

func getNumber():
	return 99

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
func scheme():
	#var s = Scheme.new()
	#s.add(10)
	#s.add(20)
	#print(s.get_total())
	#s.reset()
	#printt(s.listen(3.3))
	#printt(s.eval("(number->string (+ 1 2 3))"))
	#printt(s.repl())
	#printt(s.processInput(InputHandler.new()))
	# Inject a GDScript class function into the cpp area
	# Why doesn't "self" work properly or as expected?  Hmm...
	#s.registerFn(0, InputHandler.new(), 'getInputs')
	#printt(s.registerFn(0, self, 'getNumber'))
	pass

func load_scm():
	var s = Scheme.new()
	s.registerFn(0, xxx, "getInputs")
	var fh = File.new()
	fh.open("res://hw.scm", fh.READ)
	var scm = fh.get_as_text()
	fh.close()
	var res = s.eval(scm)
	printt(res)
	printt(s.get_total())

# Called when the node enters the scene tree for the first time.
func _ready():
	printt ("Eeeeey")
	scheme()
	load_scm()

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
