-- //-----------------------------------------------------------//
-- //---------------- Functions --------------------------------//
-- //-----------------------------------------------------------//

-- Defaults for new C++ projects
function CppProjectDefaults()
	location "%{prj.name}"
	language "C++"
	cppdialect "C++20"
	warnings "Extra"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
	}

	flags
	{
		"MultiProcessorCompile",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		defines
		{
			"GADGET_PLATFORM_WIN32",
		}

		fatalwarnings
		{
			"4715", -- Not all control paths return a value
			"4834", -- Discarding return value of function with [[nodiscard]] attribute
		}

	filter "configurations:Debug"
		defines "GADGET_DEBUG"
		symbols "On"
		runtime "Debug"
	
	filter "configurations:Develop"
		defines
		{
			"GADGET_DEBUG",
			"GADGET_DEVELOP",
		}
		
		symbols "On"
		optimize "Speed"
		runtime "Debug"
	
		flags
		{
			"LinkTimeOptimization"
		}
	
	filter "configurations:Release"
		defines "GADGET_RELEASE"
		optimize "Speed"
		runtime "Release"
	
		flags
		{
			"LinkTimeOptimization"
		}
	
	filter {} -- Deactivate filters
end

function GadgetExternalIncludes()
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
end

function GadgetExternalLibDirs()
	libdirs
		{
			"Build/SDL2/%{cfg.buildcfg}/",
			"Build/SDL2main/%{cfg.buildcfg}/",
			"Build/Glad/%{cfg.buildcfg}/",
			"SDK/Assimp/lib/x64/",
			"SDK/freetype/libs/%{cfg.buildcfg}/",
			"Build/BulletCollision/%{cfg.buildcfg}/",
			"Build/BulletDynamics/%{cfg.buildcfg}/",
			"Build/BulletLinearMath/%{cfg.buildcfg}/",
		}
end

function GadgetExternalLibs(options)
	dependson
	{
		"Glad",
		"SDL2",
		"SDL2main",
		"BulletCollision",
		"BulletDynamics",
		"BulletLinearMath",
	}

	options = options or {}
	if options.linkLibs then
		links
		{
			"assimp-vc143-mt.lib",
			"Glad.lib",
			"SDL2.lib",
			"SDL2main.lib",
			"freetype.lib",
			"BulletCollision.lib",
			"BulletDynamics.lib",
			"BulletLinearMath.lib",
		}

		filter "system:windows"
			links
			{
				"dxgi.lib",
				"d3d12.lib",
			}
	end

	filter {} --Deactivate filters
end

function DependsOnGadgetEngine()
	includedirs { "GadgetEngine/" }
	libdirs { "Build/GadgetEngine/%{cfg.buildcfg}/" }
	links { "GadgetEngine.lib" }
	dependson { "GadgetEngine" }
	forceincludes { "Gadget.h" }
end

function IgnoreDefaultLibrariesForDebug()
	ignoredefaultlibraries
	{
		"libcmt",
		"msvcrt", 
		"msvcrtd"
	}
end

function IgnoreDefaultLibrariesForRelease()
	ignoredefaultlibraries
	{
		"libcmtd",
		"msvcrt",
		"msvcrtd"
	}
end

-- //-----------------------------------------------------------//
-- //---------------------- Workspace --------------------------//
-- //-----------------------------------------------------------//

workspace "GadgetEngine"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Develop",
		"Release"
	}
	
	startproject "Game"

-- //-----------------------------------------------------------//
-- //------------------ External Projects ----------------------//
-- //-----------------------------------------------------------//

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
	
-- //-----------------------------------------------------------//
-- //--------------------- GadgetEngine ------------------------//
-- //-----------------------------------------------------------//

project "GadgetEngine"
	location "GadgetEngine"
	kind "StaticLib"

	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs()
	
	filter "system:windows"
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
		
	filter "configurations:Release"
		symbols "Off"

-- //-----------------------------------------------------------//
-- //-------------------- GadgetDLL ----------------------------//
-- //-----------------------------------------------------------//

project "GadgetDLL"
	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs { linkLibs="true" }
	DependsOnGadgetEngine()

	kind "SharedLib"
	targetname "Gadget"

	defines
	{
		"GADGETDLL_EXPORTS",
		"_WINDOWS",
		"_USRDLL"
	}

	filter "system:windows"
		buildoptions "/Zl"

		postbuildcommands
		{
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d"
		}

	filter "configurations:Debug or Develop"
		IgnoreDefaultLibrariesForDebug()

	filter "configurations:Release"
		IgnoreDefaultLibrariesForRelease()

-- //-----------------------------------------------------------//
-- //----------------------- Game ------------------------------//
-- //-----------------------------------------------------------//

project "Game"
	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs { linkLibs="true" }
	DependsOnGadgetEngine()

	dpiawareness "HighPerMonitor"
	debugdir ("%{cfg.targetdir}")
	
	filter "system:windows"
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)Build\\Resources\\\" \"$(TargetDir)Resources\\\" /y /E /d",
		}
		
	filter "configurations:Debug or Develop"
		kind "ConsoleApp"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		kind "WindowedApp"
		IgnoreDefaultLibrariesForRelease()
		
-- //-----------------------------------------------------------//
-- //------------------ Workbench ------------------------------//
-- //-----------------------------------------------------------//

externalproject "Workbench"
	location "Workbench"
	filename "Workbench"
	kind "WindowedApp"
	language "C#"
	dependson "GadgetDLL"

-- //-----------------------------------------------------------//
-- //------------------ DataBuilder ----------------------------//
-- //-----------------------------------------------------------//

project "DataBuilder"
	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs { linkLibs="true" }
	DependsOnGadgetEngine()

	kind "ConsoleApp"

	debugdir ("%{wks.location}")

	filter "system:windows"
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\dxc\\bin\\x64\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}

		libdirs
		{
			"SDK/dxc/lib/x64"
		}

		links
		{
			"dxcompiler.lib"
		}

-- //-----------------------------------------------------------//
-- //---------------------- Tests ------------------------------//
-- //-----------------------------------------------------------//

project "Tests"
		CppProjectDefaults()
		GadgetExternalIncludes()
		GadgetExternalLibDirs()
		GadgetExternalLibs{ linkLibs="true" }
		DependsOnGadgetEngine()

		kind "ConsoleApp"
		debugdir ("%{cfg.targetdir}")

		filter "system:windows"
		postbuildcommands
		{
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(SolutionDir)SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}