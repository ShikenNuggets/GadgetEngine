#ifndef GADGET_SDL2_UTILS_H
#define GADGET_SDL2_UTILS_H

#include <cstdint>

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26819) //Unnanotated fallthrough in switch statement
#include <SDL3/SDL.h>
#pragma warning(default : 26819)

#include "Input/InputEnums.h"

namespace Gadget{
	namespace SDL2_Utils{
		ButtonID ConvertSDLKeycodeToButtonID(SDL_Keycode key_);
		ButtonID ConvertSDLMouseButtonToButtonID(Uint8 button_);

		ButtonID ConvertSDLJoystickButtonToButtonID(Uint8 button_);
		AxisID ConvertSDLJoystickAxisToAxisID(Uint8 axis_);
	};
}

#endif //!GADGET_SDL2_UTILS_H