# Compiling just the module as a so

in project root:

scons scheme_shared=yes platform=x11 bin/libscheme.x11.tools.64.so

Run with:

export LD_LIBRARY_PATH=`pwd`/bin/ ; ./bin/godot.x11.tools.64
