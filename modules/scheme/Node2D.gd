extends Node2D

var xxx = InputHandler.new(self)

# Make our "lib" of useful calls.
var myDraw = MyDraw.new()

# Inject to our basic wrapper
var schemer = Schemer.new(myDraw)

# Bind it to the custom module
var s = Scheme.new()

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
	var fh = File.new()
	fh.open("res://hw.scm", fh.READ)
	var scm = fh.get_as_text()
	fh.close()
	var res = s.eval(scm)
	printt(res)
	printt(s.get_total())

# Called when the node enters the scene tree for the first time.
func _ready():
	s.registerFn(0, schemer, "sline")
	add_child(myDraw)
	scheme()
	load_scm()

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

# Solution for the below problem - make a 'proxy' class that extends nothing.
class Schemer:
	var that

	func _init(_that):
		that = _that

	func line(sx, sy, dx, dy, _x):
		printt("Called Schemer::line")
		that.line(int(sx), int(sy), int(dx), int(dy))
		that.update()

	func sline(_1, _2, _3, _4, _5):
		printt("In SLINE call for wrapper")
		that.line(200, 200, 500, 500)
		that.update()

	func line_reset():
		that.line_reset()

	func whatevs(n, _1, _2, _3, _4):
		return that.ok()
		#return that.call("ok")

# Hm, unable to send this to scheme, crashes if it receives a Node2D
# vs a barebones class.
class MyDraw extends Node2D:
	var x = 0
	var y = 0
	var lines = [
	]

	func ok():
		return "Hello from MyDraw"

	func line_reset():
		lines = []

	func line(sx, sy, dx, dy):
		lines.push_back([sx, sy, dx, dy])

	func dl(sx, sy, dx, dy):
		draw_line(Vector2(sx, sy), Vector2(dx, dy), Color(200,200,200), 2.0, false)

	func _draw():
		dl(0, 0, x, y)
		dl(0, 0, y, x)
		for l in lines:
			dl(l[0], l[1], l[2], l[3])

	func _process(delta):
		x += 2
		y += 1
		update()
