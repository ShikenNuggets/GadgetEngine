#ifndef GADGET_WIN32_UTILS_H
#define GADGET_WIN32_UTILS_H

#include <cstdint>

#include "Input/InputEnums.h"

typedef int32_t SDL_Keycode; //Yes this is correct, and it's weird
typedef uint8_t Uint8;

namespace Gadget{
	class Win32_Utils{
	public:
		static ButtonID ConvertSDLKeycodeToButtonID(SDL_Keycode key_);
		static ButtonID ConvertSDLMouseButtonToButtonID(Uint8 button_);
	};
}

#endif //!GADGET_WIN32_UTILS_H