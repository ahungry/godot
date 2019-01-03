# Compiling just the module as a so

in project root:

scons scheme_shared=yes platform=x11 bin/libscheme.x11.tools.64.so

Seems I have to also run this:

scons scheme_shared=yes platform=x11

Run with:

export LD_LIBRARY_PATH=`pwd`/bin/ ; ./bin/godot.x11.tools.64
