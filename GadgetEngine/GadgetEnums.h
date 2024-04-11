#ifndef GADGET_ENUMS_H
#define GADGET_ENUMS_H

#include <cstdint>

namespace Gadget{
	enum class ErrorCode : uint32_t{
		OK = 0,
		Unknown = 1,
		Invalid_State,
		Invalid_Args,
		FileIO,
		Constructor_Failed,

		D3D12_Error,
		D3D12_NoValidAdapter,
		D3D12_BadFeatureLevel
	};
}

#endif //!GADGET_ENUMS_H