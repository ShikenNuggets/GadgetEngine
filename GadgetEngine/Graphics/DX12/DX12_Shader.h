#ifndef GADGET_DX12_SHADER_H
#define GADGET_DX12_SHADER_H

#include "Graphics/Shader.h"
#include "Graphics/DX12/LowLevel/DX12_PSO.h"
#include "Graphics/DX12/LowLevel/DX12_RootSignature.h"

namespace Gadget{
	class DX12_Shader : public Shader{
	public:
		DX12_Shader(const std::map<ShaderType::Type, D3D12_SHADER_BYTECODE>& shaders_);

		static constexpr const char* typeName = "DX12_Shader";

		virtual size_t SizeInBytes() const override{ return sizeof(*this); } //TODO - This isn't quite right

		virtual void Bind() override{}
		virtual void Unbind() override{}

		virtual void BindInt([[maybe_unused]] StringID uniformName_, [[maybe_unused]] int value_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindFloat([[maybe_unused]] StringID uniformName_, [[maybe_unused]] float value_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindVector2([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Vector2& vec_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindVector3([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Vector3& vec_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindVector4([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Vector4& vec_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindMatrix3([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Matrix3& mat3_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindMatrix4([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Matrix4& mat4_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }
		virtual void BindColor([[maybe_unused]] StringID uniformName_, [[maybe_unused]] const Color& color_) override{ GADGET_ASSERT_NOT_IMPLEMENTED; }

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