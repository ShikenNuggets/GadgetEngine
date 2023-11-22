workspace "LIBS"
	architecture "x64"

	configurations
	{
		"Debug",
		"Develop",
		"Release"
	}
	
externalproject "BulletCollision"
	location "_prj"
	filename "BulletCollision"
	kind "StaticLib"
	language "C++"
	
externalproject "BulletDynamics"
	location "_prj"
	filename "BulletDynamics"
	kind "StaticLib"
	language "C++"
	
externalproject "BulletLinearMath"
	location "_prj"
	filename "LinearMath"
	kind "StaticLib"
	language "C++"

project "Glad"
	location "_prj/"
	kind "StaticLib"
	language "C"
	warnings "Off"

	targetdir ("../Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("../Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	files
	{
		"Glad/include/**.h",
		"Glad/src/**.c"
	}

	includedirs
	{
		"Glad/include/"	
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		runtime "Release"

		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\SDL\\include\\*.h\" \"$(SolutionDir)SDK\\_Gadget\\include\\$(ProjectName)\\\" /s /y /E",
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E"
		}
		
	filter "configurations:Debug"
		symbols "Default"
		
	filter "configurations:Develop"
		symbols "Default"
		optimize "Speed"
		
	filter "configurations:Release"
		symbols "On"
		optimize "Speed"

project "SDL2"
	location "_prj/"
	kind "SharedLib"
	language "C++"
	warnings "Off"

	targetdir ("../Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("../Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	defines { "DLL_EXPORT" }

	files
	{
		"SDL/include/**.h",

		"SDL/src/SDL.c",
		"SDL/src/SDL_**",

		"SDL/src/atomic/**",

		"SDL/src/audio/SDL_**",
		"SDL/src/audio/disk/**",
		"SDL/src/audio/dummy/**",

		"SDL/src/cpuinfo/**",

		"SDL/src/dynapi/**.c",
		"SDL/src/dynapi/**.h",

		"SDL/src/events/**",

		"SDL/src/file/SDL_**",

		"SDL/src/haptic/SDL_**",
		"SDL/src/haptic/dummy/**",

		"SDL/src/hidapi/SDL_**",
		"SDL/src/hidapi/hidapi/**",

		"SDL/src/joystick/SDL_**",
		"SDL/src/joystick/controller_type**",
		"SDL/src/joystick/usb_ids.h",
		"SDL/src/joystick/hidapi/SDL_**",
		"SDL/src/joystick/virtual/**",

		"SDL/src/libm/**",

		"SDL/src/locale/SDL_**",

		"SDL/src/misc/SDL_**",

		"SDL/src/power/SDL_**",
		
		"SDL/src/render/SDL_**",
		"SDL/src/render/software/**",

		"SDL/src/sensor/SDL_**",

		"SDL/src/stdlib/**",

		"SDL/src/thread/SDL_**",
		"SDL/src/thread/generic/SDL_syscond**",

		"SDL/src/timer/SDL_**",

		"SDL/src/video/SDL_**",
		"SDL/src/video/dummy/**",
		"SDL/src/video/khronos/vulkan/**",
		"SDL/src/video/yuv2rgb/**.h",
		"SDL/src/video/yuv2rgb/**.c",
	}

	removefiles
	{
		"SDL/include/SDL_config_**.h",
		"SDL/src/events/scancodes_**.h",
		"SDL/src/events/imKStoUCS**",
		"SDL/src/stdlib/SDL_vacopy",
	}
	
	includedirs
	{
		"SDL/include/"
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
		staticruntime "On"
		runtime "Release"
		
		defines { "_WINDOWS" }

		files
		{
			"SDL/include/SDL_config_windows.h",
			"SDL/src/events/scancodes_windows.h",
			"SDL/src/audio/directsound/**.c",
			"SDL/src/audio/directsound/**.h",
			"SDL/src/audio/wasapi/**.c",
			"SDL/src/audio/wasapi/**.h",
			"SDL/src/audio/winmm/**",
			"SDL/src/core/windows/**",
			"SDL/src/filesystem/windows/**",
			"SDL/src/haptic/windows/**",
			"SDL/src/joystick/windows/**",
			"SDL/src/loadso/windows/**",
			"SDL/src/locale/windows/**",
			"SDL/src/misc/windows/**",
			"SDL/src/power/windows/**",

			"SDL/src/render/direct3d/**",
			"SDL/src/render/direct3d11/**",
			"SDL/src/render/direct3d12/**",
			"SDL/src/render/opengl/**",
			"SDL/src/render/opengles2/**",
			
			"SDL/src/sensor/windows/**",
			"SDL/src/thread/windows/**",
			"SDL/src/timer/windows/**",
			"SDL/src/video/windows/**",
		}

		removefiles
		{
			"SDL/src/audio/wasapi/SDL_wasapi_winrt.cpp",
			"SDL/src/render/direct3d11/SDL_render_winrt**",
		}

		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\$(ProjectName)\\include\\*.h\" \"$(SolutionDir)SDK\\_Gadget\\include\\$(ProjectName)\\\" /s /y /E",
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E"
		}
		
	filter "configurations:Debug"
		symbols "Default"
		defines { "_DEBUG" }
		
	filter "configurations:Develop"
		symbols "Default"
		optimize "Speed"
		defines { "_DEBUG" }
		
	filter "configurations:Release"
		symbols "On"
		optimize "Speed"
		defines { "NDEBUG" }

project "SDL2main"
	location "_prj"
	kind "StaticLib"
	language "C++"
	warnings "Off"

	targetdir ("../Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("../Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	files
	{
		"SDL/src/main/windows/SDL_windows_main.c"
	}

	includedirs
	{
		"SDL/include/"
	}

	dependson
	{
		"SDL2",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		runtime "Release"
		
	filter "configurations:Debug"
		symbols "Default"
	
	filter "configurations:Develop"
		symbols "Default"
		optimize "Speed"
	
	filter "configurations:Release"
		symbols "On"
		optimize "Speed"