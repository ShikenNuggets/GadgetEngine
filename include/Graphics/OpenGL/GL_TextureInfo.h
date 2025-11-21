#ifndef GADGET_GL_TEXTURE_INFO_H
#define GADGET_GL_TEXTURE_INFO_H

#include <glad/glad.h>

#include "Graphics/Texture.h"
#include "Graphics/TextureInfo.h"

namespace Gadget{
	class GL_TextureInfo : public TextureInfo{
	public:
		GL_TextureInfo(const Texture& texture_);
		virtual ~GL_TextureInfo() override;

		virtual void Bind(int textureIndex_ = 0) override;
		virtual void Unbind() override;

		GLuint GetTexture() const{ return textureID; }

	private:
		GLuint textureID;
	};
}

#endif //!GADGET_GL_TEXTURE_INFO_H