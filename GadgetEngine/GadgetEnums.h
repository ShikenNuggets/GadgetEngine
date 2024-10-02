#ifndef GADGET_ENUMS_H
#define GADGET_ENUMS_H

#include <array>
#include <cstdint>
#include <string>

namespace Gadget{
	//NOTE - DO NOT make any assumptions about the underlying numeric values of error codes
	//"OK" will always be 0, "Unknown" will always be 1, and errors will always be > 0. Beyond that I make no promises
	//In the (distant) future when the engine is more stable, I'll make some adjustments here so there can be some guarantees (TODO)
	//But for now, depending on an error to have any specific value **WILL** break your code later
	enum class ErrorCode : uint32_t{
		OK = 0,
		Unknown = 1,
		//Generic Errors
		Invalid_State,
		Invalid_Args,
		FileIO,
		Constructor_Failed,
		Out_Of_Memory,

		//Platform Errors
		Win32_Error,
		Win32_FileIO_PermissionsError,
		Win32_NoAssociationError,

		//SDK Errors
		SDL_Error,

		//Graphics API Errors
		D3D12_Error,
		D3D12_NoValidAdapter,
		D3D12_BadFeatureLevel,

		OpenGL_Error,
		OpenGL_ShaderCompileFailed,

		//FMOD Errors
		FMOD_Error,
		FMOD_Init_Error,
		FMOD_Version_Error,
		FMOD_Shutdown_Error,

		//FreeType Errors
		FreeType_Errror,

		ErrorCode_MAX
	};

	//This is pretty naive, but the better solutions are kinda messy
	constexpr const char* ErrorCodeStr[] = {
		"OK",
		"Unknown",
		//Generic Errors
		"Invalid_State",
		"Invalid_Args",
		"FileIO",
		"Constructor_Failed",
		"Out_Of_Memory",

		//Platform Errors
		"Win32_Error",
		"Win32_FileIO_PermissionsError",
		"Win32_NoAssociationError",

		//SDK Errors
		"SDL_Error",

		//Graphics API Errors
		"D3D12_Error",
		"D3D12_NoValidAdapter",
		"D3D12_BadFeatureLevel",

		"OpenGL_Error",
		"OpenGL_ShaderCompileFailed",

		"FMOD_Error",
		"FMOD_Init_Error",
		"FMOD_Version_Error",
		"FMOD_Shutdown_Error",

		"FreeType_Error",

		//ErrorCode_MAX
	};

	static_assert(std::size(ErrorCodeStr) == static_cast<size_t>(ErrorCode::ErrorCode_MAX));

	//Safe function for getting the error code string
	//Invalid error codes will return "Unknown_Error_Code"
	constexpr inline std::string GetErrorCodeString(ErrorCode err_){
		if(err_ <= ErrorCode::OK || err_ >= ErrorCode::ErrorCode_MAX){
			return "Unknown_Error_Code";
		}

		return ErrorCodeStr[static_cast<size_t>(err_)];
	}

	enum class VolumeChannel : uint8_t{
		Master = 0,
		Music,
		Dialogue,
		SFX,
		Ambience,

		VolumeChannel_MAX
	};
}

#endif //!GADGET_ENUMS_H