# GadgetEngine

Yet another 3D game engine. But it's *my* 3D game engine :)  
This is mostly just a project for fun, but I want to see exactly how far I can take this thing.

At the moment I'm only trying to get it to work for Windows (x64) but I'm abstracting platform specific code where I can, so ideally this can be ported to other platforms in the future.

| Minimum Viable Product |  |
|-------------------------------------------------------------|--|
| Could theoretically be used to make a non-trivial game | *No* |

| Low Level |  |
|-------------------------------------------------------------|--|
| Proper Memory Management  |  |
| Efficient Strings (hashed string IDs) | ✓ |
| Fully Support UTF-8 Encoding  |  |
| Engine Configuration (config files, etc)  | ✓ |
| Debug Tools (logging, console commands, etc) |  |
| Engine Core (update/render loop, timing, etc) | ✓ |
| Runtime Resource Management  | ✓ |

| Input |  |
|-------------------------------------------------------------|--|
| Keyboard/Mouse | ✓ |
| Gamepads | ✓ |
| Handling Input From Multiple Gamepads |  |
| Input Gestures | ✓ |

| Rendering |  |
|-------------------------------------------------------------|--|
| Basic 3D Geometry Rendering with DX12  |  |
| Raytraced Lighting/Shadows/Reflections  |  |
| HDR
| Basic Skeletal Animation  |  |
| Facial Animation  |  |
| Debug Drawing  |  |
| Debug Cameras  |  |

| Physics/Gameplay |  |
|-------------------------------------------------------------|--|
| Basic Physics | ✓ |
| Collision Detection | ✓ |
| Game Logic Systems  | ✓ |

| Audio |  |
|-------------------------------------------------------------|--|
| Basic Audio Processing/Output |  |
| 3D Audio  |  |

| Tools (Workbench) |  |
|-------------------------------------------------------------|--|
| World Editor |  |
| Object Editor |  |
| Asset Management Tool  |  |
| Asset Conditioning Pipeline  |  |
| Profiler  |  |
| Memory Leak/Corruption Detection  |  |
| Race Condition Detection  |  |
| Non-C++ Scripting (Python? C#?) |  |
