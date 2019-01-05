# Compiling just the module as a so

https://godotengine.org/article/godot-getting-more-languages

in project root:

scons scheme_shared=yes platform=x11 bin/libscheme.x11.tools.64.so

Seems I have to also run this:

scons scheme_shared=yes platform=x11

Run with:

export LD_LIBRARY_PATH=`pwd`/bin/ ; ./bin/godot.x11.tools.64

Maybe make some new export tepmlates to put in ~/.godot/templates?

scons platform=x11 tools=no target=release

Thanks to this link! https://godotengine.org/qa/12273/custom-c-module

```GDScript
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

# Called when the node enters the scene tree for the first time.
func _ready():
	printt ("Eeeeey")
	scheme()

func _process(delta):
	# Incremented via scheme
	printt(s.get_total())
	$Label.text = str(s.get_total())
```

Connect to port localhost 12345 and try out (my-add 5) - you will see
that reflected on the game canvas in real time.

For a better experience, use Emacs' Geyser mode to connect to an
existing REPL to do this.
