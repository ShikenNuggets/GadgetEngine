#ifndef GADGET_GL_SHADER_H
#define GADGET_GL_SHADER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "Graphics/Color.h"
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

		virtual void BindInt(StringID uniformName_, int value_) override;
		virtual void BindFloat(StringID uniformName_, float value_) override;
		virtual void BindVector3(StringID uniformName_, const Vector3& vec_) override;
		virtual void BindVector4(StringID uniformName_, const Vector4& vec_) override;
		virtual void BindMatrix3(StringID uniformName_, const Matrix3& mat3_) override;
		virtual void BindMatrix4(StringID uniformName_, const Matrix4& mat4_) override;
		virtual void BindColor(StringID uniformName_, const Color& color_) override;

	private:
		GLuint shader;
		std::map<StringID, GLuint> uniforms;

		inline void AddUniform(StringID uniformName_){
			if(uniforms.find(uniformName_) == uniforms.end()){
				GADGET_ASSERT(glGetUniformLocation(GetShaderProgram(), uniformName_.GetString().c_str()) != -1, "Tried to get invalid OpenGL uniform [" + uniformName_.GetString() + "]!");
				uniforms.insert(std::make_pair(uniformName_, glGetUniformLocation(GetShaderProgram(), uniformName_.GetString().c_str())));
			}
		}

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