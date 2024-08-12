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

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.hpp",
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
		gRootPath .. "%{prj.name}/",
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
		gRootPath .. "Build/SDL2/%{cfg.buildcfg}/",
		gRootPath .. "Build/SDL2main/%{cfg.buildcfg}/",
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
			"SDL2_image.lib",
			"freetype.lib",
			"BulletCollision.lib",
			"BulletDynamics.lib",
			"BulletLinearMath.lib",
			"fsbank_vc.lib",
		}

		filter "system:windows"
			links
			{
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