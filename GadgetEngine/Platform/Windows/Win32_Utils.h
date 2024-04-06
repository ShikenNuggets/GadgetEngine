#ifndef GADGET_WIN32_UTILS_H
#define GADGET_WIN32_UTILS_H

#include <string>

#include "Input/InputEnums.h"

typedef int32_t SDL_Keycode; //Yes this is correct, and it's weird
typedef uint8_t Uint8;

namespace Gadget{
	namespace Win32_Utils{
		std::string GetUserDocumentsPath();
	};
}

#endif //!GADGET_WIN32_UTILS_H