workspace "GadgetEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "Game"

externalproject "SDL2"
	location "SDK/SDL/VisualC/SDL"
	filename "SDL"
	kind "SharedLib"
	language "C++"
	
externalproject "SDL2main"
	location "SDK/SDL/VisualC/SDLmain"
	filename "SDLmain"
	kind "SharedLib"
	language "C++"
	dependson
	{
		"SDL2",
	}

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