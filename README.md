# GadgetEngine

Yet another 3D game engine. But it's *my* 3D game engine :)  
This is mostly just a project for fun, but I want to see exactly how far I can take this thing.

At the moment I'm only trying to get it to work for Windows (x64) but I'm abstracting platform specific code where I can, so ideally this can be ported to other platforms in the future.

Currently relies on SDL2 so I can focus on the engine itself, but I'm hoping to eliminate this dependency at some point.

| Minimum Viable Product (in no particular order) |  |
|-------------------------------------------------------------|--|
| Could theoretically be used to make a non-trivial game | *No* |
| Proper Memory Management  |  |
| Efficient Strings (hashed string IDs) | ✓ |
| Fully Support UTF-8 Encoding  |  |
| Engine Configuration (config files, console commands, etc)  |  |
| Asset Management (at runtime + tool for engine users)  |  |
| HID Input (gamepads, keyboard, mouse, etc) | ✓ |
| Debug Tools (logging, drawing, menus, console commands, etc) |  |
| Basic 3D Rendering with OpenGL (geometry, lighting/shadows, etc)  |  |
| Basic Skeletal Animation  |  |
| Collision Detection |  |
| Physics |  |
| Basic Audio Processing/Output |  |
| 3D Audio  |  |
| Non-C++ Game Data (levels/game objects/etc) and scripting (C#?) |  |
| Tools for Engine Users (world/object editor, etc) |  |
