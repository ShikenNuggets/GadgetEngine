#ifndef GADGET_RENDER_COMPONENT_H
#define GADGET_RENDER_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/Materials/Material.h"

namespace Gadget{
	class RenderComponent : public Component{
	public:
		RenderComponent(GameObject* parent_, StringID modelName_, StringID textureName_, StringID shaderName_);
		RenderComponent(GameObject* parent_, StringID modelName_, const Color& color_, StringID shaderName_);
		RenderComponent(GameObject* parent_, StringID modelName_, Material* material_);
		virtual ~RenderComponent() override;

		void Bind();
		void Unbind();

		inline constexpr size_t GetMeshNumIndices() const{ return meshInfo->GetNumIndices(); }
		Shader* GetShader(){ return material->GetShader(); }

	private:
		MeshInfo* meshInfo;
		Material* material;

		void CreateMeshInfo(StringID modelName_);
	};
}

#endif //!GADGET_RENDER_COMPONENT_H