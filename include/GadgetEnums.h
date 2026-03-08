#ifndef GADGET_ENUMS_H
#define GADGET_ENUMS_H

#include <array>
#include <cstdint>
#include <string>

namespace Gadget{
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