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

		virtual void OnTransformModified() override;

		static RenderComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<RenderComponent*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		void Bind(size_t index_);
		void Unbind(size_t index_);

		inline constexpr size_t GetNumSubmeshes() const{ return meshInfos.size(); }

		inline constexpr MeshInfo* GetMeshInfo(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Tried to access invalid mesh at index " + std::to_string(index_));
				return nullptr;
			}

			GADGET_BASIC_ASSERT(meshInfos[index_] != nullptr);
			return meshInfos[index_];
		}

		inline constexpr MeshInstanceInfo* GetMeshInstanceInfo() const{ return meshInstanceInfo; }

		inline constexpr size_t GetMeshNumIndices(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Tried to access invalid mesh at index " + std::to_string(index_));
				return 0;
			}

			GADGET_BASIC_ASSERT(meshInfos[index_] != nullptr);
			return meshInfos[index_]->GetNumIndices();
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
		std::vector<MeshInfo*> meshInfos;
		Material* material;
		EngineMaterial* engineMaterial;
		MeshInstanceInfo* meshInstanceInfo;

		void CreateMeshInstanceInfo();

		static ComponentCollection<RenderComponent> componentCollection;
	};
}

#endif //!GADGET_RENDER_COMPONENT_H