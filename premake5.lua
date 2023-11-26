workspace "GadgetEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Develop",
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
	
externalproject "BulletCollision"
	location "SDK/_prj"
	filename "BulletCollision"
	kind "StaticLib"
	language "C++"
	
externalproject "BulletDynamics"
	location "SDK/_prj"
	filename "BulletDynamics"
	kind "StaticLib"
	language "C++"
	
externalproject "BulletLinearMath"
	location "SDK/_prj"
	filename "LinearMath"
	kind "StaticLib"
	language "C++"
	
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
		"SDK/Assimp/include",
		"SDK/Glad/include",
		"SDK/SDL/include",
		"SDK/freetype/include",
		"SDK/bullet3/src",
	}
	
	libdirs
	{
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
		"SDK/Assimp/lib/x64/",
		"SDK/freetype/libs/%{cfg.buildcfg}/",
		"Build/BulletCollision/%{cfg.buildcfg}/",
		"Build/BulletDynamics/%{cfg.buildcfg}/",
		"Build/BulletLinearMath/%{cfg.buildcfg}/",
	}
	
	dependson
	{
		"Glad",
		"SDL2",
		"SDL2main",
		"BulletCollision",
		"BulletDynamics",
		"BulletLinearMath",
	}
	
	flags
	{
		"MultiProcessorCompile",
	}
	
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		staticruntime "On"
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}

		fatalwarnings
		{
			"4715"
		}

		postbuildcommands
		{
			"echo D|xcopy \"$(ProjectDir)*.h*\" \"$(SolutionDir)SDK\\_Gadget\\include\\$(ProjectName)\\\" /s /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\include\\*.h*\" \"$(SolutionDir)SDK\\_Gadget\\include\\common\\\" /s /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\Assimp\\include\\*.*\" \"$(SolutionDir)SDK\\_Gadget\\include\\Assimp\\\" /s /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\freetype\\include\\*.*\" \"$(SolutionDir)SDK\\_Gadget\\include\\freetype\\\" /s /y /E /d",
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\Assimp\\bin\\x64\\*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\Assimp\\lib\\x64\\*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\freetype\\libs\\$(Configuration)\\*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d"
		}
		
	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		runtime "Debug"
		
	filter "configurations:Develop"
		defines "GADGET_DEBUG"
		symbols "On"
		optimize "Speed"
		runtime "Debug"
		
		flags
		{
			"LinkTimeOptimization"
		}
		
		libdirs
		{
			"Build/GadgetEngine/Debug/",
			"Build/Glad/Debug/",
			"Build/SDL2/Debug/",
			"Build/SDL2main/Debug/",
			"SDK/Assimp/lib/x64/",
			"SDK/freetype/libs/Debug/",
			"Build/BulletCollision/Debug/",
			"Build/BulletDynamics/Debug/",
			"Build/BulletLinearMath/Debug/",
		}
		
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		symbols "Off"
		optimize "Speed"
		runtime "Release"
		
		flags
		{
			"LinkTimeOptimization",
		}
		
project "GadgetDLL"
	location "GadgetDLL"
	kind "SharedLib"
	language "C++"
	warnings "Extra"

	targetname "Gadget"
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	defines
	{
		"GADGETDLL_EXPORTS",
		"_WINDOWS",
		"_USRDLL"
	}

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
		"SDK/Assimp/include",
		"SDK/Glad/include",
		"SDK/SDL/include/",
		"SDK/freetype/include",
		"SDK/bullet3/src",
	}

	libdirs
	{
		"Build/GadgetEngine/%{cfg.buildcfg}/",
		"Build/Glad/%{cfg.buildcfg}/",
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
		"SDK/Assimp/lib/x64/",
		"SDK/freetype/libs/%{cfg.buildcfg}/",
		"Build/BulletCollision/%{cfg.buildcfg}/",
		"Build/BulletDynamics/%{cfg.buildcfg}/",
		"Build/BulletLinearMath/%{cfg.buildcfg}/",
	}

	links
	{
		"GadgetEngine.lib",
		"assimp-vc143-mt.lib",
		"Glad.lib",
		"SDL2.lib",
		"SDL2main.lib",
		"freetype.lib",
		"BulletCollision.lib",
		"BulletDynamics.lib",
		"BulletLinearMath.lib",
	}

	dependson
	{
		"GadgetEngine",
		"Glad",
		"SDL2",
		"SDL2main",
		"BulletCollision",
		"BulletDynamics",
		"BulletLinearMath",
	}

	flags
	{
		"MultiProcessorCompile"
	}

	forceincludes
	{
		"Gadget.h"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		staticruntime "On"
		buildoptions "/Zl"
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}

		fatalwarnings
		{
			"4715"
		}

		postbuildcommands
		{
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d"
		}
		
	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		runtime "Debug"
		
	filter "Configurations:Develop"
		defines "GADGET_DEBUG"
		symbols "On"
		optimize "Speed"
		runtime "Debug"
		
		flags
		{
			"LinkTimeOptimization"
		}
		
		libdirs
		{
			"Build/GadgetEngine/Debug/",
			"Build/Glad/Debug/",
			"Build/SDL2/Debug/",
			"Build/SDL2main/Debug/",
			"SDK/Assimp/lib/x64/",
			"SDK/freetype/libs/Debug/",
			"Build/BulletCollision/Debug/",
			"Build/BulletDynamics/Debug/",
			"Build/BulletLinearMath/Debug/",
		}
		
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		optimize "Speed"
		runtime "Release"
		
		flags
		{
			"LinkTimeOptimization"
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
		"SDK/Assimp/include",
		"SDK/Glad/include",
		"SDK/SDL/include/",
		"SDK/freetype/include",
		"SDK/bullet3/src",
	}
	
	libdirs
	{
		"Build/GadgetEngine/%{cfg.buildcfg}/",
		"Build/Glad/%{cfg.buildcfg}/",
		"Build/SDL2/%{cfg.buildcfg}/",
		"Build/SDL2main/%{cfg.buildcfg}/",
		"SDK/Assimp/lib/x64/",
		"SDK/freetype/libs/%{cfg.buildcfg}/",
		"Build/BulletCollision/%{cfg.buildcfg}/",
		"Build/BulletDynamics/%{cfg.buildcfg}/",
		"Build/BulletLinearMath/%{cfg.buildcfg}/",
	}
	
	links
	{
		"GadgetEngine.lib",
		"assimp-vc143-mt.lib",
		"Glad.lib",
		"SDL2.lib",
		"SDL2main.lib",
		"freetype.lib",
		"BulletCollision.lib",
		"BulletDynamics.lib",
		"BulletLinearMath.lib",
	}
	
	dependson
	{
		"GadgetEngine",
		"Glad",
		"SDL2",
		"SDL2main",
		"BulletCollision",
		"BulletDynamics",
		"BulletLinearMath",
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	forceincludes
	{
		"Gadget.h"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		staticruntime "On"
		
		
		defines
		{
			"GADGET_PLATFORM_WIN32",
		}

		fatalwarnings
		{
			"4715"
		}
		
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)Build\\Resources\\\" \"$(TargetDir)Resources\\\" /y /E /d",
		}
		
	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		kind "ConsoleApp"
		runtime "Debug"
		
	filter "Configurations:Develop"
		defines "GADGET_DEBUG"
		symbols "On"
		optimize "Speed"
		kind "ConsoleApp"
		runtime "Debug"
		
		flags
		{
			"LinkTimeOptimization"
		}
		
		libdirs
		{
			"Build/GadgetEngine/Debug/",
			"Build/Glad/Debug/",
			"Build/SDL2/Debug/",
			"Build/SDL2main/Debug/",
			"SDK/Assimp/lib/x64/",
			"SDK/freetype/libs/Debug/",
			"Build/BulletCollision/Debug/",
			"Build/BulletDynamics/Debug/",
			"Build/BulletLinearMath/Debug/",
		}
		
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		optimize "Speed"
		kind "WindowedApp"
		runtime "Release"
		
		flags
		{
			"LinkTimeOptimization"
		}
		
externalproject "Workbench"
	location "Workbench"
	filename "Workbench"
	kind "WindowedApp"
	language "C#"
	dependson "GadgetDLL"