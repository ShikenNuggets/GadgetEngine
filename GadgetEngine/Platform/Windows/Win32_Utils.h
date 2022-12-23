#ifndef GADGET_WIN32_UTILS_H
#define GADGET_WIN32_UTILS_H

#include <string>

#include "Input/InputEnums.h"

typedef int32_t SDL_Keycode; //Yes this is correct, and it's weird
typedef uint8_t Uint8;

namespace Gadget{
	namespace Win32_Utils{
		ButtonID ConvertSDLKeycodeToButtonID(SDL_Keycode key_);
		ButtonID ConvertSDLMouseButtonToButtonID(Uint8 button_);

		ButtonID ConvertSDLJoystickButtonToButtonID(Uint8 button_);
		AxisID ConvertSDLJoystickAxisToAxisID(Uint8 axis_);

		std::string GetUserDocumentsPath();
	};
}

#endif //!GADGET_WIN32_UTILS_H