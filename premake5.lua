workspace "GadgetEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release"
	}
	
	startproject "Game"

externalproject "Glad"
	location "SDK/_prj"
	filename "Glad"
	kind "StaticLib"
	language "C"

externalproject "SDL2"
	location "SDK/_prj"
	filename "SDL2"
	kind "SharedLib"
	language "C++"
	
externalproject "SDL2main"
	location "SDK/_prj"
	filename "SDL2main"
	kind "StaticLib"
	language "C++"
	dependson
	{
		"SDL2",
	}
	
project "GadgetEngine"
	location "GadgetEngine"
	kind "StaticLib"
	language "C++"
	warnings "Extra"
	
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
		"SDK/Glad/include",
		"SDK/SDL/include",
	}
	
	libdirs
	{
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
	}
	
	dependson
	{
		"Glad",
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
		staticruntime "On"
		runtime "Release"
		
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
	warnings "Extra"
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
		"SDK/Glad/include",
		"SDK/SDL/include/",
	}
	
	libdirs
	{
		"Build/GadgetEngine/%{cfg.buildcfg}/",
		"Build/Glad/%{cfg.buildcfg}/",
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
	}
	
	links
	{
		"GadgetEngine.lib",
		"Glad.lib",
		"SDL2.lib",
		"SDL2main.lib",
	}
	
	dependson
	{
		"GadgetEngine",
		"Glad",
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
		staticruntime "On"
		runtime "Release"
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}
		
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)Build\\SDL2\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E",
			"echo D|xcopy \"$(SolutionDir)Build\\Resources\\\" \"$(TargetDir)\\Resources\\\" /y /E /d",
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