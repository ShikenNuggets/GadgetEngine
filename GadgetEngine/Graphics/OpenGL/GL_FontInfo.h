#ifndef GADGET_GL_FONT_INFO_H
#define GADGET_GL_FONT_INFO_H

#include <glad/glad.h>

#include "Graphics/FontInfo.h"
#include "Graphics/Text/FreetypeFont.h"

namespace Gadget{
	class GL_FontInfo : public FontInfo{
	public:
		GL_FontInfo(const FreetypeFont& font_);

		GLuint GetTextureForChar(char c_){
			GADGET_BASIC_ASSERT(textures.find(c_) != textures.end());
			return textures.at(c_); //TODO - Default blank character for unknown chars?
		}

	private:
		std::map<char, GLuint> textures;
	};
}

#endif //!GADGET_GL_FONT_INFO_H