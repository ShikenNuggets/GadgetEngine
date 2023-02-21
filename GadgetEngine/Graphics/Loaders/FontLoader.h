#ifndef GADGET_FONT_LOADER_H
#define GADGET_FONT_LOADER_H

#include <string>
#include <vector>

#include "Math/Vector.h"

namespace Gadget{
	//Forward Declaration
	class Font;

	class FontLoader{
	public:
		static Font* LoadFont(const std::string& filePath_);
	};
}

#endif //!GADGET_FONT_LOADER_H