#include "GL_FontInfo.h"

using namespace Gadget;

GL_FontInfo::GL_FontInfo(const FreetypeFont& font_) : FontInfo(){
	//Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	const auto& characters = font_.GetCharacters();
	for(const auto& c : characters){
		GLuint texture;
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			static_cast<GLsizei>(c.second.width),
			static_cast<GLsizei>(c.second.rows),
			0, GL_RED, GL_UNSIGNED_BYTE,
			c.second.bitmapBuffer.data()
		);

		//Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		textures.emplace(c.first, texture);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	//Reset byte alignment to initial value
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}