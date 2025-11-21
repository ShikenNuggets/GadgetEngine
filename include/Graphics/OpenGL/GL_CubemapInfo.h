#ifndef GADGET_GL_CUBEMAP_INFO_H
#define GADGET_GL_CUBEMAP_INFO_H

#include <glad/glad.h>

#include "Graphics/Cubemap.h"
#include "Graphics/TextureInfo.h"

namespace Gadget{
	class GL_CubemapInfo : public TextureInfo{
	public:
		GL_CubemapInfo(const Cubemap& cubemap_);
		virtual ~GL_CubemapInfo() override;

		virtual void Bind(int textureIndex_ = 0) override;
		virtual void Unbind() override;

	private:
		GLuint textureID;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};
}

#endif //!GADGET_GL_CUBEMAP_INFO_H