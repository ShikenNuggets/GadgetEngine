#ifndef GADGET_GL_SHADER_H
#define GADGET_GL_SHADER_H

#include <string>
#include <glad/glad.h>

#include "Graphics/Shader.h"
#include "Math/Matrix.h"

namespace Gadget{
	class GL_Shader : public Shader{
	public:
		GL_Shader(const std::string& vertPath_, const std::string& fragPath_);
		virtual ~GL_Shader() override;

		GLuint GetShaderProgram();
		virtual void Bind() override;
		virtual void Unbind() override;

		void BindMatrix4(StringID uniformName_, const Matrix4& mat4_);

	private:
		GLuint shader;

		std::string GetShaderLog(GLuint shader_);
	};

	class GL_ShaderResourceContainer : public ResourceContainer{
	public:
		GL_ShaderResourceContainer(const std::string& vertPath_, const std::string fragPath_) : vertPath(vertPath_), fragPath(fragPath_){}

		virtual Resource* LoadResource() override{
			resource = new GL_Shader(vertPath, fragPath);
			loadCount++;
			return resource;
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(GL_Shader);
		}

	private:
		std::string vertPath;
		std::string fragPath;
	};
}

#endif //!GADGET_GL_SHADER_H