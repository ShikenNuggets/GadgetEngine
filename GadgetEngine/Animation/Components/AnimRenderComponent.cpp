#include "AnimRenderComponent.h"

#include "App.h"
#include "Game/GameObject.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"

using namespace Gadget;

ComponentCollection<AnimRenderComponent> AnimRenderComponent::componentCollection = ComponentCollection<AnimRenderComponent>();

AnimRenderComponent::AnimRenderComponent(GameObject* parent_, StringID modelName_, std::vector<StringID> cachedMaterials_) : Component(SID("AnimRenderComponent"), parent_), modelName(modelName_), meshInfos(), animator(nullptr){
	GADGET_BASIC_ASSERT(parent != nullptr && parent->GetGUID() != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	for(const auto& cm : cachedMaterials_){
		GADGET_BASIC_ASSERT(cm != StringID::None);
		GADGET_BASIC_ASSERT(GetCachedMaterial(cm) != nullptr);
	}

	CreateMeshInfo();
	CreateAnimator();

	for(size_t i = 0; i < meshInfos.size(); i++){
		size_t indexToUse = i;
		if(indexToUse >= cachedMaterials_.size()){
			indexToUse = 0;
		}

		meshInfos[i].second = cachedMaterials_[indexToUse];
	}

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

AnimRenderComponent::AnimRenderComponent(GameObject* parent_, StringID modelName_, StringID cachedMaterial_) : Component(SID("AnimRenderComponent"), parent_), modelName(modelName_), meshInfos(), animator(nullptr){
	GADGET_BASIC_ASSERT(parent != nullptr && parent->GetGUID() != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(cachedMaterial_ != StringID::None);
	GADGET_BASIC_ASSERT(GetCachedMaterial(cachedMaterial_) != nullptr);

	CreateMeshInfo();
	CreateAnimator();

	for(auto& m : meshInfos){
		m.second = cachedMaterial_;
	}

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

AnimRenderComponent::AnimRenderComponent(const ComponentProperties& props_) : Component(props_){
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	AnimRenderComponent::Deserialize(props_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

AnimRenderComponent::~AnimRenderComponent(){
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

	delete animator;

	for(const auto& m : meshInfos){
		delete m.first;
	}

	componentCollection.Remove(this);

	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
}

void AnimRenderComponent::Update(float deltaTime_){
	if(animator != nullptr){
		animator->Update(deltaTime_);
	}
}

void AnimRenderComponent::Bind(size_t index_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());

	meshInfos[index_].first->Bind();
	GetCachedMaterial(index_)->Bind();
}

void AnimRenderComponent::Unbind(size_t index_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());

	GetCachedMaterial(index_)->Unbind();
	meshInfos[index_].first->Unbind();
}

void AnimRenderComponent::AddClip(StringID clipName_){
	if(animator != nullptr){
		animator->AddClip(clipName_);
	}else{
		GADGET_LOG_WARNING(SID("ANIM"), "Tried to add animation clip to an AnimRenderComponent with no Animator");
	}
}

void AnimRenderComponent::CreateMeshInfo(){
	GADGET_BASIC_ASSERT(modelName != StringID::None);

	AnimMesh* mesh = App::GetResourceManager().LoadResource<AnimMesh>(modelName);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	const std::vector<MeshInfo*> mi = App::GetRenderer().GenerateAPIAnimMeshInfos(*mesh);
	GADGET_BASIC_ASSERT(!mi.empty());
	for(const auto& m : mi){
		meshInfos.emplace_back(m, StringID::None);
	}
	GADGET_BASIC_ASSERT(!meshInfos.empty());

	App::GetResourceManager().UnloadResource(modelName);
}

void AnimRenderComponent::CreateAnimator(){
	AnimMesh* mesh = App::GetResourceManager().LoadResource<AnimMesh>(modelName);
	animator = new Animator(modelName, mesh->skeleton, {});
}

ComponentProperties AnimRenderComponent::Serialize() const{
	ComponentProperties props = Component::Serialize();
	props.variables.Add(SID("ModelName"), modelName);
	GetCachedMaterial(0)->Serialize(props.variables); //TODO - Serialize multiple materials

	return props;
}

void AnimRenderComponent::Deserialize(const ComponentProperties& props_){
	modelName = props_.variables.GetValue(SID("ModelName"), StringID::None).ToStr();
	const StringID materialType = props_.variables.GetValue(SID("MaterialType"), StringID::None).ToStr();
	const StringID shaderName = props_.variables.GetValue(SID("ShaderName"), StringID::None).ToStr();

	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(materialType != StringID::None);
	GADGET_BASIC_ASSERT(shaderName != StringID::None);

	CreateMeshInfo();
	CreateAnimator();

	//TODO - This is all very dumb
	if(materialType == DiffuseTextureMaterial::type){
		const StringID materialName = StringID::ProcessString("__" + std::to_string(GetGUID().Id()) + "_DiffuseTextureMaterial");
		App::GetMaterialCache().AddMaterial(materialName, new DiffuseTextureMaterial(props_.variables));
		for(auto& m : meshInfos){
			m.second = materialName;
		}
	}else if(materialType == ColorMaterial::type){
		const StringID materialName = StringID::ProcessString("__" + std::to_string(GetGUID().Id()) + "_ColorMaterial");
		App::GetMaterialCache().AddMaterial(materialName, new ColorMaterial(props_.variables));
		for(auto& m : meshInfos){
			m.second = materialName;
		}
	}else{
		GADGET_ASSERT_NOT_IMPLEMENTED;
	}
}

Material* AnimRenderComponent::GetCachedMaterial(size_t meshIndex_) const{
	return GetCachedMaterial(meshInfos[meshIndex_].second);
}

Material* AnimRenderComponent::GetCachedMaterial(StringID materialName_) const{
	GADGET_BASIC_ASSERT(App::GetMaterialCache().GetMaterial(materialName_) != nullptr);
	return App::GetMaterialCache().GetMaterial(materialName_);
}