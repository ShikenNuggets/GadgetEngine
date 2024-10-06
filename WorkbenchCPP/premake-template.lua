gRootPath = ""
dofile("premake/common.lua")

workspace "__MACRO__PROJECT__NAME__"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Develop",
		"Release"
	}
	
	startproject "__MACRO__PROJECT__NAME__"

project "__MACRO__PROJECT__NAME__"
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
			"echo D|xcopy \"SDK\\_Gadget\\lib\\$(Configuration)\\*.dll\" \"$(TargetDir)\" /y /E /d",
			"echo D|xcopy \"SDK\\_Gadget\\lib\\$(Configuration)\\*.pdb\" \"$(TargetDir)\" /y /E /d",
		}
		
	filter "configurations:Debug or Develop"
		kind "ConsoleApp"
		IgnoreDefaultLibrariesForDebug()
		
	filter "configurations:Release"
		kind "WindowedApp"
		IgnoreDefaultLibrariesForRelease()