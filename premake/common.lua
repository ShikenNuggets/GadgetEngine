--EXPECTS
--gRootPath = "PathToThing/" --The trailing slash is important

-- Defaults for new C++ projects
function CppProjectDefaults()
	location "%{prj.name}"
	language "C++"
	cppdialect "C++20"
	warnings "Extra"
	
	targetdir ("Build/%{prj.name}/%{cfg.buildcfg}/") 
	objdir ("Build/Intermediate/%{prj.name}/%{cfg.buildcfg}/")

	defines
	{
		"SDL_FUNCTION_POINTER_IS_VOID_POINTER"
	}

	files
	{
		"%{prj.location}/**.h",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.cpp",
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
			"GADGET_FAST_ASSERTS",
		}
		
		symbols "On"
		optimize "Speed"
		runtime "Debug"
	
		flags
		{
			"LinkTimeOptimization"
		}
	
	filter "configurations:Release"
		defines
		{
			"GADGET_RELEASE",
			"GADGET_FAST_ASSERTS",
		}
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
		gRootPath .. "SDK/include",
		gRootPath .. "SDK/Assimp/include",
		gRootPath .. "SDK/Glad/include",
		gRootPath .. "SDK/SDL/include",
		gRootPath .. "SDK/freetype/include",
		gRootPath .. "SDK/bullet3/src",
		gRootPath .. "SDK/FMOD/core/inc",
		gRootPath .. "SDK/FMOD/fsbank/inc",
		gRootPath .. "SDK/FMOD/studio/inc",
	}
end

function GadgetExternalLibDirs()
	libdirs
	{
		gRootPath .. "Build/SDL3/%{cfg.buildcfg}/",
		gRootPath .. "Build/Glad/%{cfg.buildcfg}/",
		gRootPath .. "SDK/Assimp/lib/x64/",
		gRootPath .. "SDK/freetype/libs/%{cfg.buildcfg}/",
		gRootPath .. "SDK/lib/",
		gRootPath .. "Build/BulletCollision/%{cfg.buildcfg}/",
		gRootPath .. "Build/BulletDynamics/%{cfg.buildcfg}/",
		gRootPath .. "Build/BulletLinearMath/%{cfg.buildcfg}/",
		gRootPath .. "SDK/FMOD/core/lib/",
		gRootPath .. "SDK/FMOD/fsbank/lib/",
		gRootPath .. "SDK/FMOD/studio/lib/",
	}
end

function GadgetUserExternalIncludes()
	includedirs
	{
		gRootPath .. "SDK/_Gadget/include/Assimp",
		gRootPath .. "SDK/_Gadget/include/bullet3",
		gRootPath .. "SDK/_Gadget/include/common",
		gRootPath .. "SDK/_Gadget/include/FMOD",
		gRootPath .. "SDK/_Gadget/include/freetype",
		gRootPath .. "SDK/_Gadget/include/GadgetEngine",
		gRootPath .. "SDK/_Gadget/include/Glad",
		gRootPath .. "SDK/_Gadget/include/SDL2",
	}
end

function GadgetUserExternalLibDirs()
	libdirs
	{
		gRootPath .. "SDK/_Gadget/lib/%{cfg.buildcfg}/"
	}
end

function GadgetExternalLibs(options)
	dependson
	{
		"Glad",
		"SDL",
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
			"SDL3.lib",
			"SDL3_image.lib",
			"freetype.lib",
			"BulletCollision.lib",
			"BulletDynamics.lib",
			"BulletLinearMath.lib",
			"fsbank_vc.lib",
		}

		filter "system:windows"
			links
			{
				"dwmapi.lib",
				"dxgi.lib",
				"d3d12.lib",
			}

		filter "configurations:Debug or Develop"
			links
			{
				"fmodL_vc.lib",
				"fmodstudioL_vc.lib",
			}

		filter "configurations:Release"
			links
			{
				"fmod_vc.lib",
				"fmodstudio_vc.lib",
			}
	end

	filter {} --Deactivate filters
end

function DependsOnGadgetEngine()
	includedirs { gRootPath .. "GadgetEngine/" }
	libdirs { gRootPath .. "Build/GadgetEngine/%{cfg.buildcfg}/" }
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