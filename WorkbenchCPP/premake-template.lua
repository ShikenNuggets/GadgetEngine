gRootPath = ""
dofile("premake/common.lua")

workspace "__MACRO_PROJECT_NAME__"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Develop",
		"Release"
	}
	
	startproject "__MACRO_PROJECT_NAME__"

project "__MACRO_PROJECT_NAME__"
	CppProjectDefaults()
	GadgetExternalIncludes()
	GadgetExternalLibDirs()
	GadgetExternalLibs { linkLibs="true" }
	DependsOnGadgetEngine()

	location "__MACRO_PROJECT_PATH__"

	dpiawareness "HighPerMonitor"
	debugdir ("%{cfg.targetdir}")
	
	filter "system:windows"
		postbuildcommands
		{
			"echo D|xcopy \"__WORKBENCH_DIR__\\SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"__WORKBENCH_DIR__\\SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}
		
	filter "configurations:Debug or Develop"
		kind "ConsoleApp"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		kind "WindowedApp"
		IgnoreDefaultLibrariesForRelease()