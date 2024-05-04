#ifndef GADGET_ENUMS_H
#define GADGET_ENUMS_H

#include <array>
#include <cstdint>
#include <string>

namespace Gadget{
	enum class ErrorCode : uint32_t{
		OK = 0,
		Unknown = 1,
		//Generic Errors
		Invalid_State,
		Invalid_Args,
		FileIO,
		Constructor_Failed,

		//Platform Errors
		Win32_Error,

		//SDK Errors
		SDL_Error,

		//Graphics API Errors
		D3D12_Error,
		D3D12_NoValidAdapter,
		D3D12_BadFeatureLevel,

		OpenGL_Error,
		OpenGL_ShaderCompileFailed,

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

		//Platform Errors
		"Win32_Error",

		//SDK Errors
		"SDL_Error",

		//Graphics API Errors
		"D3D12_Error",
		"D3D12_NoValidAdapter",
		"D3D12_BadFeatureLevel",

		"OpenGL_Error",
		"OpenGL_ShaderCompileFailed",

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
}

#endif //!GADGET_ENUMS_H