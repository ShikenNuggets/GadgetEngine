# GadgetEngine

Yet another 3D game engine. But it's *my* 3D game engine :)  
This is mostly just a project for fun, but I want to see exactly how far I can take this thing.

At the moment I'm only trying to get it to work for Windows (x64) but I'm abstracting platform specific code where I can to make it easier (in theory) to port to other platforms in the future.

More details can be found [on my portfolio website](https://gamesbycarter.wordpress.com/2022/06/22/gadget-engine-c/).

Progress
---
| [Minimum Viable Product Milestone](https://github.com/ShikenNuggets/GadgetEngine/milestone/1) |  |
|-------------------------------------------------------------|--|
| Could theoretically be used to make a non-trivial game | *No* |

Build Instructions
---
Windows is the only officially supported platforms at this time. Windows 10 (May 2021 or newer) and 11 are supported.
1. Clone the repository using the Git client of your choice (I like [TortoiseGit](https://tortoisegit.org/). Consider [SourceTree](https://www.sourcetreeapp.com/) or [GitHub Desktop](https://desktop.github.com/) if you're looking for a more beginner-friendly option).

2. Install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/). Older versions are not supported.
   * In the Visual Studio Installer, select the following workloads:
     * .NET desktop development
     * Desktop development with C++
     * Game development with C++
   * In the installation details panel under ".NET desktop development", add the following optional components:
       * Development tools for .NET
       * .NET profiling tools
   * Under "Desktop development for C++":
       * MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
       * C++ profiling tools
       * C++ AddressSanitizer
       * Windows 10 or 11 SDK (10.0.20348.0 or newer)
   * Under "Game development with C++":
       * HLSL Tools
    
   * Head over to Individual components, and add the following:
       * .NET - .NET 8.0 Runtime (Long Term Support)
       * .NET - .NET SDK
       * .NET - NuGet package manager
       * Compilers, build tools, and runtimes - Windows Universal CRT SDK
       * Games and Graphics - Graphics debugger and GPU profiler for DirectX
       * SDKs, libraries, and frameworks - Windows Universal C Runtime

3. Run _premake.bat_. This isn't strictly necessary since the Visual Studio solution and project files are included in the repository, but it's good practice to ensure that everything is working and up to date.
4. Open _GadgetEngine.sln_ in Visual Studio 2022. Set your startup project (Game for testing the runtime, Workbench for the editor), and build the solution, but **do not run the Game project yet**. Performing a batch build of all projects and configurations is recommended at this point. At minimum, ensure that DataBuilder is built in the Release configuration, as this is required for the next step.
5. Run _build-resources.bat_. This will build shaders and copy them and other game assets to all the relevant directories.
6. You should now be able to run game.

Contributions
---
As this is a primarily personal/educational project and I wish to implement all the major engine features myself, I am not looking for outside contributions at this time. I will consider accepting outside contributions once the core engine is in a more stable state.

That said, for any issues marked as ["Help Wanted"](https://github.com/ShikenNuggets/GadgetEngine/issues?q=is%3Aopen+is%3Aissue+label%3A%22Help+Wanted%22), I appreciate any insight anyone has.
