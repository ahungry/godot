extends Node2D

var xxx = InputHandler.new(self)

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
	var d = MyDraw.new()
	add_child(d)

func _process(delta):
	pass
	# Incremented via scheme
	# printt(s.get_total())
	# $Label.text = str(s.get_total())

class InputHandler:
	var that

	func _init(_that):
		that = _that

	func getInputs():
		printt("I was called from somewhere!")
		return that.getNumber() + 1

# var game = Game.new()
# game.processInput(InputHandler.new())

class MyDraw extends Node2D:
	var x = 0
	var y = 0

	func line(sx, sy, dx, dy):
		draw_line(Vector2(sx, sy), Vector2(dx, dy), Color(200,200,200), 2.0, false)

	func _draw():
		line(0, 0, x, y)
		line(0, 0, y, x)

	func _process(delta):
		x += 2
		y += 1
		update()
