#ifndef GADGET_RENDER_COMPONENT_H
#define GADGET_RENDER_COMPONENT_H

#include "Data/Pair.h"
#include "Game/Component.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/MeshInstanceInfo.h"
#include "Graphics/Materials/EngineMaterial.h"
#include "Graphics/Materials/Material.h"

namespace Gadget{
	class RenderComponent : public Component{
	public:
		RenderComponent(GUID parentGUID_, StringID modelName_, StringID cachedMaterial_);
		RenderComponent(GUID parentGUID_, StringID modelName_, std::vector<StringID> cachedMaterials_);
		RenderComponent(GUID parentGUID_, StringID modelName_, EngineMaterial* engineMaterial_, bool setMeshInfoDeferred_ = true);
		RenderComponent(const ComponentProperties& props_);

		virtual ~RenderComponent() override;

		virtual void OnTransformModified() override;

		static RenderComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static const ComponentCollection<RenderComponent>& GetCollection(){ return componentCollection; }

		void Bind(size_t index_);
		void Unbind(size_t index_);

		inline constexpr size_t GetNumSubmeshes() const{ return meshInfos.size(); }

		inline constexpr MeshInfo* GetMeshInfo(size_t index_){
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Tried to access invalid mesh at index " + std::to_string(index_));
				return nullptr;
			}

			if(meshInfos[index_].first == nullptr){
				CreateMeshInfo();
			}

			GADGET_BASIC_ASSERT(meshInfos[index_].first != nullptr);
			return meshInfos[index_].first;
		}

		inline constexpr MeshInstanceInfo* GetMeshInstanceInfo() const{
			return nullptr;
			//return meshInstanceInfo;
		}

		inline constexpr size_t GetMeshNumIndices(size_t index_) const{
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Tried to access invalid mesh at index " + std::to_string(index_));
				return 0;
			}

			GADGET_BASIC_ASSERT(meshInfos[index_].first != nullptr);
			return meshInfos[index_].first->GetNumIndices();
		}

		Material* GetMaterial(size_t index_){
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Trying to access invalid material at index " + std::to_string(index_));
				return nullptr;
			}

			GADGET_BASIC_ASSERT(GetCachedMaterial(index_) != nullptr);
			return GetCachedMaterial(index_);
		}

		EngineMaterial* GetEngineMaterial(){
			GADGET_ASSERT_NOT_IMPLEMENTED;
			return nullptr;
			//return engineMaterial;
		}

		void SetMaterial(StringID cachedMaterial);
		void SetMaterial(size_t index_, StringID cachedMaterial);

		Shader* GetShader(size_t index_){
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Trying to access material at invalid index " + std::to_string(index_));
				return nullptr;
			}
			
			Material* material = GetCachedMaterial(index_);
			GADGET_BASIC_ASSERT(material != nullptr);
			if(material == nullptr){
				GADGET_LOG_WARNING(SID("RENDER"), "No valid material cached at index !" + std::to_string(index_));
				return nullptr;
			}

			return material->GetShader();
		}

		virtual ComponentProperties Serialize() const override;

		void CreateMeshInfo();
		void InvalidateMeshInfo();

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

		Material* GetCachedMaterial(size_t meshIndex_) const;
		static Material* GetCachedMaterial(StringID material_);

	private:
		StringID modelName;
		std::vector<Pair<MeshInfo*, StringID>> meshInfos;
		//EngineMaterial* engineMaterial;
		//MeshInstanceInfo* meshInstanceInfo;

		void CreateMeshInstanceInfo();

		static ComponentCollection<RenderComponent> componentCollection;
	};
}

#endif //!GADGET_RENDER_COMPONENT_H