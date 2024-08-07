#ifndef STAR_HAWX_DEFINES_H
#define STAR_HAWX_DEFINES_H

#include <Utils/StringID.h>

namespace StarHawx{
	static const Gadget::StringID UpDownMoveAxis = SID("UpDownMove");
	static const Gadget::StringID LeftRightMoveAxis = SID("LeftRightMove");
	static const Gadget::StringID MenuButton = SID("Menu");
	static const Gadget::StringID ShootButton = SID("Shoot");

	enum class TiltDir : uint8_t{
		None,
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		UpLeft,

		TiltDir_MAX
	};
}

#endif //!STAR_HAWX_DEFINES_H