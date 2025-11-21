#ifndef GADGET_FREETYPE_FONT_LOADER_H
#define GADGET_FREETYPE_FONT_LOADER_H

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Math/Vector.h"

namespace Gadget{
	//Forward Declaration
	class FreetypeFont;

	class FreetypeFontLoader{
	public:
		FreetypeFontLoader();
		~FreetypeFontLoader();

		FreetypeFont* LoadFont(const std::string& filePath_) const;

	private:
		FT_Library ftLib;
	};
}

#endif //!GADGET_FREETYPE_FONT_LOADER_H