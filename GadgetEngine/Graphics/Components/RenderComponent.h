#ifndef GADGET_RENDER_COMPONENT_H
#define GADGET_RENDER_COMPONENT_H

#include "Game/Component.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/MeshInstanceInfo.h"
#include "Graphics/Materials/EngineMaterial.h"
#include "Graphics/Materials/Material.h"

namespace Gadget{
	class RenderComponent : public Component{
	public:
		RenderComponent(GUID parentGUID_, StringID modelName_, StringID textureName_, StringID shaderName_);
		RenderComponent(GUID parentGUID_, StringID modelName_, const Color& color_, StringID shaderName_);
		RenderComponent(GUID parentGUID_, StringID modelName_, Material* material_);
		RenderComponent(GUID parentGUID_, StringID modelName_, EngineMaterial* engineMaterial_, bool setMeshInfoDeferred_ = true);
		RenderComponent(const ComponentProperties& props_);

		virtual ~RenderComponent() override;

		static RenderComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<RenderComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		void Bind();
		void Unbind();

		inline constexpr MeshInfo* GetMeshInfo() const{ return meshInfo; }
		inline constexpr MeshInstanceInfo* GetMeshInstanceInfo() const{ return meshInstanceInfo; }

		inline constexpr size_t GetMeshNumIndices() const{
			GADGET_BASIC_ASSERT(meshInfo != nullptr);
			return meshInfo->GetNumIndices();
		}

		Material* GetMaterial(){ return material; }
		EngineMaterial* GetEngineMaterial(){ return engineMaterial; }

		Shader* GetShader(){
			GADGET_BASIC_ASSERT(material != nullptr);
			return material->GetShader();
		}

		virtual ComponentProperties Serialize() const override;

		void CreateMeshInfo();

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		StringID modelName;
		MeshInfo* meshInfo;
		Material* material;
		EngineMaterial* engineMaterial;
		MeshInstanceInfo* meshInstanceInfo;

		void CreateMeshInstanceInfo();

		static ComponentCollection<RenderComponent> componentCollection;
	};
}

#endif //!GADGET_RENDER_COMPONENT_H