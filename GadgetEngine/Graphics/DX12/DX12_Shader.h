#ifndef GADGET_DX12_SHADER_H
#define GADGET_DX12_SHADER_h

#include "Graphics/Shader.h"
#include "Graphics/DX12/LowLevel/DX12_PSO.h"
#include "Graphics/DX12/LowLevel/DX12_RootSignature.h"

namespace Gadget{
	class DX12_Shader : public Shader{
	public:
		DX12_Shader(const std::map<ShaderType::Type, D3D12_SHADER_BYTECODE>& shaders_);

		static constexpr const char* typeName = "DX12_Shader";

		virtual void Bind() override{}
		virtual void Unbind() override{}

		virtual void BindInt(StringID uniformName_, int value_) override{}
		virtual void BindFloat(StringID uniformName_, float value_) override{}
		virtual void BindVector2(StringID uniformName_, const Vector2& vec_) override{}
		virtual void BindVector3(StringID uniformName_, const Vector3& vec_) override{}
		virtual void BindVector4(StringID uniformName_, const Vector4& vec_) override{}
		virtual void BindMatrix3(StringID uniformName_, const Matrix3& mat3_) override{}
		virtual void BindMatrix4(StringID uniformName_, const Matrix4& mat4_) override{}
		virtual void BindColor(StringID uniformName_, const Color& color_) override{}

	private:
		DX12_RootSignature rootSignature;
		DX12_PSO pso;
	};

	class DX12_ShaderResourceContainer : public ResourceContainer{
	public:
		DX12_ShaderResourceContainer(EngineShader::ID vertex_, EngineShader::ID pixel_) : ResourceContainer(DX12_Shader::typeName, ""), vertex(vertex_), pixel(pixel_){}

		virtual Resource* LoadResource() override{
			std::map<ShaderType::Type, D3D12_SHADER_BYTECODE> shaders;
			shaders.emplace(ShaderType::Type::Vertex, DX12_ShaderHandler::GetEngineShader(vertex));
			shaders.emplace(ShaderType::Type::Pixel, DX12_ShaderHandler::GetEngineShader(pixel));

			return new DX12_Shader(shaders);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(DX12_Shader);
		}

	private:
		EngineShader::ID vertex;
		EngineShader::ID pixel;
	};
}

#endif //!GADGET_DX12_SHADER_H