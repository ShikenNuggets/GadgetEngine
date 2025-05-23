#ifndef GADGET_GL_SHADER_H
#define GADGET_GL_SHADER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "Debug.h"
#include "Graphics/Color.h"
#include "Graphics/Shader.h"
#include "Math/Matrix.h"

namespace Gadget{
	class GL_Shader : public Shader{
	public:
		GL_Shader(const std::string& vertPath_, const std::string& fragPath_);
		virtual ~GL_Shader() override;

		static constexpr const char* typeName = "GL_Shader";

		virtual size_t SizeInBytes() const override{ return sizeof(*this); } //TODO - Memory dynamically allocated for uniforms is not considered

		GLuint GetShaderProgram() const;
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void BindInt(StringID uniformName_, int value_) override;
		virtual void BindFloat(StringID uniformName_, float value_) override;
		virtual void BindVector2(StringID uniformName_, const Vector2& vec_) override;
		virtual void BindVector3(StringID uniformName_, const Vector3& vec_) override;
		virtual void BindVector4(StringID uniformName_, const Vector4& vec_) override;
		virtual void BindMatrix3(StringID uniformName_, const Matrix3& mat3_) override;
		virtual void BindMatrix4(StringID uniformName_, const Matrix4& mat4_) override;
		virtual void BindColor(StringID uniformName_, const Color& color_) override;

	private:
		GLuint shader;
		std::map<StringID, GLint> uniforms;

		inline bool HasUniform(StringID uniformName_){
			const auto& find = uniforms.find(uniformName_);
			if(find != uniforms.end()){
				return find->second != -1;
			}

			const auto& insertedPair = uniforms.insert(std::make_pair(uniformName_, glGetUniformLocation(GetShaderProgram(), uniformName_.GetString().c_str())));
			if(!insertedPair.second){
				Debug::Log("Failed to insert StringID \"" + uniformName_.GetString() + "\" into uniform map!", Debug::Error);
				return false;
			}

			return insertedPair.first->second != -1;
		}

		static std::string GetShaderLog(GLuint shader_);
		static std::string GetProgramLog(GLuint program_);
	};

	class GL_ShaderResourceContainer : public ResourceContainer{
	public:
		GL_ShaderResourceContainer(const std::string& vertPath_, const std::string fragPath_) : ResourceContainer(GL_Shader::typeName, vertPath_), fragPath(fragPath_){
			GADGET_ASSERT_FILE_EXISTS(vertPath_);
			GADGET_ASSERT_FILE_EXISTS(fragPath_);
		}

		std::string FragPath() const{ return fragPath; }

		virtual Resource* LoadResource() override{
			return new GL_Shader(path, fragPath);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(GL_Shader);
		}

	private:
		std::string fragPath;
	};
}

#endif //!GADGET_GL_SHADER_H