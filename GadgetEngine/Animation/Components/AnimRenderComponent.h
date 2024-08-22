#ifndef GADGET_ANIM_RENDER_COMPONENT_H
#define GADGET_ANIM_RENDER_COMPONENT_H

#include "Animation/AnimMesh.h"
#include "Data/Pair.h"
#include "Game/Component.h"
#include "Graphics/MeshInfo.h"
#include "Graphics/Materials/Material.h"

namespace Gadget{
	class AnimRenderComponent : public Component{
	public:
		AnimRenderComponent(GameObject* parent_, StringID modelName_, std::vector<StringID> cachedMaterials_);
		AnimRenderComponent(GameObject* parent_, StringID modelName_, StringID cachedMaterial_);
		AnimRenderComponent(const ComponentProperties& props_);

		virtual ~AnimRenderComponent() override;

		static AnimRenderComponent* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<AnimRenderComponent*> GetComponents(GUID objectGuid_){
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

			GADGET_BASIC_ASSERT(meshInfos[index_].first != nullptr);
			return meshInfos[index_].first;
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

		Shader* GetShader(size_t index_){
			GADGET_BASIC_ASSERT(index_ < meshInfos.size());
			if(index_ >= meshInfos.size()){
				GADGET_LOG_WARNING(SID("RENDER"), "Trying to access invalid material at index " + std::to_string(index_));
				return nullptr;
			}

			GADGET_BASIC_ASSERT(GetCachedMaterial(index_) != nullptr);
			return GetCachedMaterial(index_)->GetShader();
		}

		virtual ComponentProperties Serialize() const override;

		void CreateMeshInfo();

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

		Material* GetCachedMaterial(size_t meshIndex_) const;
		Material* GetCachedMaterial(StringID material_) const;

	private:
		StringID modelName;
		std::vector<Pair<MeshInfo*, StringID>> meshInfos;

		static ComponentCollection<AnimRenderComponent> componentCollection;
	};
}

#endif //!GADGET_ANIM_RENDER_COMPONENT_H