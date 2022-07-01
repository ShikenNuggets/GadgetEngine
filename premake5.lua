workspace "GadgetEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "Game"
	
project "SDL2"
	location "SDK/_prj"
	kind "SharedLib"
	language "C++"

	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	files
	{
		"SDK/SDL/include/**.h",
		"SDK/SDL/src/**.c",
		"SDK/SDL/src/**.h"
	}

	removefiles "SDK/SDL/include/SDL_config_**.h"

	removefiles "SDK/SDL/src/**/android/**"
	removefiles "SDK/SDL/src/**/bsd/**"
	removefiles "SDK/SDL/src/**/emscripten/**"
	removefiles "SDK/SDL/src/**/iphoneos/**"
	removefiles "SDK/SDL/src/**/linux/**"
	removefiles "SDK/SDL/src/**/ngage/**"
	removefiles "SDK/SDL/src/**/os2/**"
	removefiles "SDK/SDL/src/**/ps2/**"
	removefiles "SDK/SDL/src/**/psp/**"
	removefiles "SDK/SDL/src/**/pthread/**"
	removefiles "SDK/SDL/src/**/qnx/**"
	removefiles "SDK/SDL/src/**/riscos/**"
	removefiles "SDK/SDL/src/**/steam/**"
	removefiles "SDK/SDL/src/**/unix/**"
	removefiles "SDK/SDL/src/**/vita/**"
	removefiles "SDK/SDL/src/**/windows/**"

	removefiles "SDK/SDL/src/audio/SDL_audio_resampler_filter.h"
	removefiles "SDK/SDL/src/audio/aaudio/**"
	removefiles "SDK/SDL/src/audio/alsa/**"
	removefiles "SDK/SDL/src/audio/android/**"
	removefiles "SDK/SDL/src/audio/arts/**"
	removefiles "SDK/SDL/src/audio/coreaudio/**"
	removefiles "SDK/SDL/src/audio/dsp/**"
	removefiles "SDK/SDL/src/audio/emscripten/**"
	removefiles "SDK/SDL/src/audio/esd/**"
	removefiles "SDK/SDL/src/audio/fusionsound/**"
	removefiles "SDK/SDL/src/audio/haiku/**"
	removefiles "SDK/SDL/src/audio/jack/**"
	removefiles "SDK/SDL/src/audio/nacl/**"
	removefiles "SDK/SDL/src/audio/nas/**"
	removefiles "SDK/SDL/src/audio/netbsd/**"
	removefiles "SDK/SDL/src/audio/openslES/**"
	removefiles "SDK/SDL/src/audio/os2/**"
	removefiles "SDK/SDL/src/audio/paudio/**"
	removefiles "SDK/SDL/src/audio/pipewire/**"
	removefiles "SDK/SDL/src/audio/ps2/**"
	removefiles "SDK/SDL/src/audio/psp/**"
	removefiles "SDK/SDL/src/audio/pulseaudio/**"
	removefiles "SDK/SDL/src/audio/qsa/**"
	removefiles "SDK/SDL/src/audio/sndio/**"
	removefiles "SDK/SDL/src/audio/sun/**"
	removefiles "SDK/SDL/src/audio/vita/**"
	removefiles "SDK/SDL/src/audio/wasapi/**"
	removefiles "SDK/SDL/src/audio/winmm/**"

	removefiles "SDK/SDL/src/core/**"

	removefiles "SDK/SDL/src/main/**"

	removefiles "SDK/SDL/src/hidapi/android/**"
	removefiles "SDK/SDL/src/hidapi/hidtest/**"
	removefiles "SDK/SDL/src/hidapi/ios/**"
	removefiles "SDK/SDL/src/hidapi/libusb/**"
	removefiles "SDK/SDL/src/hidapi/linux/**"
	removefiles "SDK/SDL/src/hidapi/m4/**"
	removefiles "SDK/SDL/src/hidapi/mac/**"
	removefiles "SDK/SDL/src/hidapi/testgui/**"
	removefiles "SDK/SDL/src/hidapi/windows/**"

	removefiles "SDK/SDL/src/events/imKStoUCS.c"

	removefiles "SDK/SDLsrc/filesystem/dummy/**"

	includedirs
	{
		"SDK/SDL/include/"
	}

	links
	{
		"setupapi.lib",
		"winmm.lib",
		"imm32.lib",
		"version.lib"
	}

	filter "system:windows"
		systemversion "latest"
		files
		{
			"SDK/SDL/include/SDL_config_windows.h",
			"SDK/SDL/src/audio/directsound/**",
			"SDK/SDL/src/audio/wasapi/**",
			"SDK/SDL/src/audio/winmm/**",
			"SDK/SDL/src/core/windows/**",
			"SDK/SDL/src/main/windows/**",
			"SDK/SDL/src/hidapi/windows/**",
			"SDK/SDL/src/joystick/windows/**",
			"SDK/SDL/src/loadso/windows/**",
			"SDK/SDL/src/locale/windows/**",
			"SDK/SDL/src/main/windows/**",
			"SDK/SDL/src/misc/windows/**",
			"SDK/SDL/src/power/windows/**",
			"SDK/SDL/src/sensor/windows/**",
			"SDK/SDL/src/thread/windows/**",
			"SDK/SDL/src/timer/windows/**",
			"SDK/SDL/src/video/windows/**"
		}
		
	filter "configurations:Debug"
		symbols "On"
		
	filter "configurations:Release"
		symbols "On"
		optimize "On"
		
		flags
		{
			"LinkTimeOptimization",
		}

project "SDL2main"
	location "SDK/_prj"
	kind "SharedLib"
	language "C++"

	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	files
	{
		"SDK/SDL/src/main/windows/SDL_windows_main.c"
	}

	includedirs
	{
		"SDK/SDL/include/"
	}

	dependson
	{
		"SDL2",
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		
	filter "configurations:Release"

project "GadgetEngine"
	location "GadgetEngine"
	kind "StaticLib"
	language "C++"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/",
		"SDK/include",
		"SDK/SDL/include",
	}
	
	libdirs
	{
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
	}
	
	dependson
	{
		"SDL2",
		"SDL2main",
	}
	
	flags
	{
		"MultiProcessorCompile",
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}
		
	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		optimize "On"
		
		flags
		{
			"LinkTimeOptimization",
		}
		
project "Game"
	location "Game"
	language "C++"
	dpiawareness "HighPerMonitor"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate//%{prj.name}/%{cfg.buildcfg}/") 
	debugdir ("%{cfg.targetdir}")
	
	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/",
		"GadgetEngine/",
		"SDK/include/",
		"SDK/SDL/include/",
	}
	
	libdirs
	{
		"Build/GadgetEngine/%{cfg.buildcfg}/",
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
	}
	
	links
	{
		"GadgetEngine.lib",
		"SDL2.lib",
		"SDL2main.lib",
	}
	
	dependson
	{
		"GadgetEngine",
		"SDL2",
		"SDL2main",
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}
		
		prebuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}
		
	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		kind "ConsoleApp"
		
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		optimize "On"
		kind "WindowedApp"
		
		flags
		{
			"LinkTimeOptimization"
		}