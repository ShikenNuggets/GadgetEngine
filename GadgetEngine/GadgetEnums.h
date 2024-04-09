#ifndef GADGET_ENUMS_H
#define GADGET_ENUMS_H

namespace Gadget{
	enum class ErrorCode : uint32_t{
		OK = 0,
		Unknown = 1,
		D3D12_Error,
		Invalid_State
	};
}

#endif //!GADGET_ENUMS_H