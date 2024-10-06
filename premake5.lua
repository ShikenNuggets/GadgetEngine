-- //-----------------------------------------------------------//
-- //---------------- Functions --------------------------------//
-- //-----------------------------------------------------------//

gRootPath = ""
dofile("premake/common.lua")

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
			"echo D|xcopy \"$(ProjectDir)*.h*\" \"$(ProjectDir)..\\SDK\\_Gadget\\include\\$(ProjectName)\\\" /s /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\include\\*.h*\" \"$(ProjectDir)..\\SDK\\_Gadget\\include\\common\\\" /s /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\Assimp\\include\\*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\include\\Assimp\\\" /s /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\freetype\\include\\*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\include\\freetype\\\" /s /y /E /d",
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\Assimp\\bin\\x64\\*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\Assimp\\lib\\x64\\*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\freetype\\libs\\$(Configuration)\\*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\lib\\*.dll\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\FMOD\\core\\lib\\*.dll\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\FMOD\\fsbank\\lib\\*.dll\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\FMOD\\studio\\lib\\*.dll\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\" /y /E /d",
		}
		
	filter "configurations:Release"
		symbols "Off"

-- //-----------------------------------------------------------//
-- //-------------------- GadgetDLL ----------------------------//
-- //-----------------------------------------------------------//

--[[
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
			"echo D|xcopy \"$(TargetDir)*.*\" \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\\" /y /E /d"
		}

	filter "configurations:Debug or Develop"
		IgnoreDefaultLibrariesForDebug()

	filter "configurations:Release"
		IgnoreDefaultLibrariesForRelease()
]]--

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
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\Resources\\\" \"$(TargetDir)Resources\\\" /y /E /d",
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

--[[
externalproject "Workbench"
	location "Workbench"
	filename "Workbench"
	kind "WindowedApp"
	language "C#"
	dependson "GadgetDLL"
]]--

project "WorkbenchCPP"
	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs{ linkLibs="true" }
	DependsOnGadgetEngine()

	dpiawareness "HighPerMonitor"
	debugdir ("%{cfg.targetdir}")

	files
	{
		"SDK/ImGui/*.h",
		"SDK/ImGui/*.hpp",
		"SDK/ImGui/*.cpp",

		"SDK/ImGui/backends/imgui_impl_opengl3.h",
		"SDK/ImGui/backends/imgui_impl_opengl3.cpp",
		"SDK/ImGui/backends/imgui_impl_sdl2.h",
		"SDK/ImGui/backends/imgui_impl_sdl2.cpp",
	}

	includedirs
	{
		"SDK/ImGui/"
	}

	filter "system:windows"
		postbuildcommands
		{
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\Resources\\*.ttf\" \"$(TargetDir)Resources\\Fonts\\\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\premake\\*.*\" \"$(TargetDir)premake\\\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)premake-template.lua\" \"$(TargetDir)\" /y /d",
		}

	filter "configurations:Debug or Develop"
		kind "ConsoleApp"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		kind "WindowedApp"
		IgnoreDefaultLibrariesForRelease() 

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
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\dxc\\bin\\x64\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}

		libdirs
		{
			"SDK/dxc/lib/x64"
		}

		links
		{
			"dxcompiler.lib"
		}

	filter "configurations:Debug or Develop"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		IgnoreDefaultLibrariesForRelease()

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
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"$(ProjectDir)..\\SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}

	filter "configurations:Debug or Develop"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		IgnoreDefaultLibrariesForRelease()