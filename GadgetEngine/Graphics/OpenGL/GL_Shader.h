#ifndef GADGET_GL_SHADER_H
#define GADGET_GL_SHADER_H

#include <string>
#include <glad/glad.h>

#include "Graphics/Shader.h"

namespace Gadget{
	class GL_Shader : public Shader{
	public:
		GL_Shader();
		virtual ~GL_Shader() override;

		GLuint GetShaderProgram();
		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		GLuint shader;

		std::string GetShaderLog(GLuint shader_);
	};
}

#endif //!GADGET_GL_SHADER_H