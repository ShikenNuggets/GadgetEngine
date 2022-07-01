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

	defines { "DLL_EXPORT" }

	files
	{
		"SDK/SDL/include/begin_code.h",
		"SDK/SDL/include/end_code.h"
	}

	files
	{
		"SDK/SDL/include/**.h",
		"SDK/SDL/src/**.c",
		"SDK/SDL/src/**.h",
		"SDK/SDL/src/**.cpp",
		"SDK/SDL/src/**.hpp"
	}

	removefiles "SDK/SDL/include/SDL_config_**.h"

	removefiles "SDK/SDL/src/**/android/**"
	removefiles "SDK/SDL/src/**/arm/**"
	removefiles "SDK/SDL/src/**/bsd/**"
	removefiles "SDK/SDL/src/**/cocoa/**"
	removefiles "SDK/SDL/src/**/darwin/**"
	removefiles "SDK/SDL/src/**/emscripten/**"
	removefiles "SDK/SDL/src/**/haiku/**"
	removefiles "SDK/SDL/src/**/iphoneos/**"
	removefiles "SDK/SDL/src/**/linux/**"
	removefiles "SDK/SDL/src/**/macosx/**"
	removefiles "SDK/SDL/src/**/nacl/**"
	removefiles "SDK/SDL/src/**/ngage/**"
	removefiles "SDK/SDL/src/**/os2/**"
	removefiles "SDK/SDL/src/**/pandora/**"
	removefiles "SDK/SDL/src/**/ps2/**"
	removefiles "SDK/SDL/src/**/psp/**"
	removefiles "SDK/SDL/src/**/pthread/**"
	removefiles "SDK/SDL/src/**/qnx/**"
	removefiles "SDK/SDL/src/**/raspberry/**"
	removefiles "SDK/SDL/src/**/riscos/**"
	removefiles "SDK/SDL/src/**/steam/**"
	removefiles "SDK/SDL/src/**/unix/**"
	removefiles "SDK/SDL/src/**/vita/**"
	removefiles "SDK/SDL/src/**/windows/**"
	removefiles "SDK/SDL/src/**/winrt/**"

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

	removefiles "SDK/SDL/src/filesystem/dummy/**"

	removefiles "SDK/SDL/src/events/imKStoUCS.h"

	removefiles "SDK/SDL/src/events/scancodes_**.h"

	removefiles "SDK/SDL/src/file/cocoa/**"

	removefiles "SDK/SDL/src/power/uikit/**"

	removefiles "SDK/SDL/src/render/direct3d11/SDL_render_winrt.h"

	removefiles "SDK/SDL/src/render/metal/**"
	removefiles "SDK/SDL/src/render/opengles/**"
	removefiles "SDK/SDL/src/render/vitagxm/**"

	removefiles "SDK/SDL/src/sensor/coremotion/**"
	removefiles "SDK/SDL/src/sensor/windows/**"

	removefiles "SDK/SDL/src/stdlib/SDL_vacopy.h"

	removefiles "SDK/SDL/src/thread/stdcpp/**"
	removefiles "SDK/SDL/src/thread/windows/**"

	removefiles "SDK/SDL/video/SDL_rect_impl.h"
	removefiles "SDK/SDL/src/video/directfb/**"
	removefiles "SDK/SDL/src/video/khronos/EGL/**"
	removefiles "SDK/SDL/src/video/khronos/GLES2/**"
	removefiles "SDK/SDL/src/video/khronos/KHR/**"
	removefiles "SDK/SDL/src/video/kmsdrm/**"
	removefiles "SDK/SDL/src/video/offscreen/**"
	removefiles "SDK/SDL/src/video/uikit/**"
	removefiles "SDK/SDL/src/video/vivante/**"
	removefiles "SDK/SDL/src/video/wayland/**"
	removefiles "SDK/SDL/src/video/x11/**"
	removefiles "SDK/SDL/src/video/yuv2rgb/yuv_rgb_lsx_func.h"

	removefiles "SDK/SDL/src/joystick/hidapi/SDL_hidapi_steam.c"

	removefiles "SDK/SDL/src/loadso/**"

	removefiles "SDK/SDL/src/locale/**"

	removefiles "SDK/SDL/src/misc/dummy/**"

	removefiles "SDK/SDL/src/render/direct3d11/SDL_render_winrt.cpp"

	removefiles "SDK/SDL/src/sensor/dummy/SDL_dummysensor.c"

	removefiles "SDK/SDL/src/test/**"

	removefiles "SDK/SDL/src/thread/generic/SDL_sysmutex.c"
	removefiles "SDK/SDL/src/thread/generic/SDL_syssem.c"
	removefiles "SDK/SDL/src/thread/generic/SDL_systhread.c"
	removefiles "SDK/SDL/src/thread/generic/SDL_systls.c"

	removefiles "SDK/SDL/src/timer/dummy/SDL_systimer.c"

	files
	{
		"SDK/SDL/src/locale/SDL_locale.c",
		"SDK/SDL/src/locale/SDL_syslocale.h"
	}
	
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
		
		defines { "_WINDOWS" }

		files
		{
			"SDK/SDL/include/SDL_config_windows.h",
			"SDK/SDL/src/audio/directsound/**.c",
			"SDK/SDL/src/audio/directsound/**.h",
			"SDK/SDL/src/audio/wasapi/**.c",
			"SDK/SDL/src/audio/wasapi/**.h",
			"SDK/SDL/src/audio/winmm/**.c",
			"SDK/SDL/src/audio/winmm/**.h",
			"SDK/SDL/src/core/windows/**.c",
			"SDK/SDL/src/core/windows/**.h",
			"SDK/SDL/src/filesystem/windows/**.c",
			"SDK/SDL/src/filesystem/windows/**.h",
			"SDK/SDL/src/haptic/windows/**.c",
			"SDK/SDL/src/haptic/windows/**.h",
			"SDK/SDL/src/hidapi/windows/**.c",
			"SDK/SDL/src/hidapi/windows/**.h",
			"SDK/SDL/src/joystick/windows/**.c",
			"SDK/SDL/src/joystick/windows/**.h",
			"SDK/SDL/src/loadso/windows/**.c",
			"SDK/SDL/src/loadso/windows/**.h",
			"SDK/SDL/src/locale/windows/**.c",
			"SDK/SDL/src/locale/windows/**.h",
			"SDK/SDL/src/misc/windows/**.c",
			"SDK/SDL/src/misc/windows/**.h",
			"SDK/SDL/src/power/windows/**.c",
			"SDK/SDL/src/power/windows/**.h",
			"SDK/SDL/src/sensor/windows/**.c",
			"SDK/SDL/src/sensor/windows/**.h",
			"SDK/SDL/src/thread/windows/**.c",
			"SDK/SDL/src/thread/windows/**.h",
			"SDK/SDL/src/timer/windows/**.c",
			"SDK/SDL/src/timer/windows/**.h",
			"SDK/SDL/src/video/windows/**.c",
			"SDK/SDL/src/video/windows/**.h",
			"SDK/SDL/src/events/scancodes_windows.h",
			"SDK/SDL/src/sensor/windows/**.c",
			"SDK/SDL/src/sensor/windows/**.h"
		}

		removefiles
		{
			"SDK/SDL/src/audio/wasapi/SDL_wasapi_winrt.cpp",
			"SDK/SDL/src/thread/generic/SDL_sysmutex_c.h",
			"SDK/SDL/src/thread/generic/SDL_systhread_c.h",
			"SDK/SDL/src/hidapi/windows/hid.c"
		}
		
	filter "configurations:Debug"
		symbols "On"

		defines { "_DEBUG" }
		
	filter "configurations:Release"
		symbols "On"
		optimize "On"
		
		defines { "NDEBUG" }

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