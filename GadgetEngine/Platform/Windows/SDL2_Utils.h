#ifndef GADGET_SDL2_UTILS_H
#define GADGET_SDL2_UTILS_H

#include <cstdint>

#include "Input/InputEnums.h"

typedef int32_t SDL_Keycode; //Yes this is correct, and it's weird
typedef uint8_t Uint8;

namespace Gadget{
	namespace SDL2_Utils{
		ButtonID ConvertSDLKeycodeToButtonID(SDL_Keycode key_);
		ButtonID ConvertSDLMouseButtonToButtonID(Uint8 button_);

		ButtonID ConvertSDLJoystickButtonToButtonID(Uint8 button_);
		AxisID ConvertSDLJoystickAxisToAxisID(Uint8 axis_);
	};
}

#endif //!GADGET_SDL2_UTILS_H