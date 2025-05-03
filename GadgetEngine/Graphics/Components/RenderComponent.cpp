#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

ComponentCollection<RenderComponent> RenderComponent::componentCollection = ComponentCollection<RenderComponent>();

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, StringID cachedMaterial_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), engineMaterial(nullptr), meshInstanceInfo(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(cachedMaterial_ != StringID::None);
	GADGET_BASIC_ASSERT(GetCachedMaterial(cachedMaterial_) != nullptr);

	CreateMeshInfo();
	CreateMeshInstanceInfo();

	for(auto& m : meshInfos){
		m.second = cachedMaterial_;
	}

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, std::vector<StringID> cachedMaterials_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), engineMaterial(nullptr), meshInstanceInfo(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(!cachedMaterials_.empty());
	for(const auto& cm : cachedMaterials_){
		GADGET_BASIC_ASSERT(GetCachedMaterial(cm) != nullptr);
	}

	CreateMeshInfo();
	CreateMeshInstanceInfo();

	for(size_t i = 0; i < meshInfos.size(); i++){
		size_t indexToUse = i;
		if(indexToUse >= cachedMaterials_.size()){
			indexToUse = 0;
		}
		
		meshInfos[i].second = cachedMaterials_[indexToUse];
	}

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, EngineMaterial* material_, bool setMeshInfoDeferred_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), engineMaterial(material_), meshInstanceInfo(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);

	if(!setMeshInfoDeferred_){
		CreateMeshInfo();
	}
	CreateMeshInstanceInfo();

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(setMeshInfoDeferred_ || !meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(engineMaterial != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(const ComponentProperties& props_) : Component(props_), modelName(StringID::None), meshInfos(), engineMaterial(nullptr), meshInstanceInfo(nullptr){
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	RenderComponent::Deserialize(props_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::~RenderComponent(){
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

	delete engineMaterial;

	delete meshInstanceInfo;

	for(const auto& m : meshInfos){
		delete m.first;
	}

	componentCollection.Remove(this);

	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
}

void RenderComponent::OnTransformModified(){
	GADGET_BASIC_ASSERT(parent != nullptr);
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);

	if(meshInstanceInfo != nullptr && parent != nullptr){
		meshInstanceInfo->Update(parent->GetTransformMatrix());
	}
}

void RenderComponent::Bind(size_t index_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());

	GetMeshInfo(index_)->Bind();
	GetCachedMaterial(index_)->Bind();
}

void RenderComponent::Unbind(size_t index_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());

	GetCachedMaterial(index_)->Unbind();
	meshInfos[index_].first->Unbind();
}

void RenderComponent::CreateMeshInfo(){
	GADGET_BASIC_ASSERT(modelName != StringID::None);

	InvalidateMeshInfo();
	auto oldMeshInfos = meshInfos;
	meshInfos.clear();

	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	const std::vector<MeshInfo*> mi = App::GetRenderer().GenerateAPIMeshInfos(*mesh);
	GADGET_BASIC_ASSERT(!mi.empty());
	for(const auto& m : mi){
		meshInfos.emplace_back(m, StringID::None);
	}
	GADGET_BASIC_ASSERT(!meshInfos.empty());

	App::GetResourceManager().UnloadResource(modelName);

	for(size_t i = 0; i < oldMeshInfos.size() && i < meshInfos.size(); i++){
		meshInfos[i].second = oldMeshInfos[i].second;
	}
}

void RenderComponent::InvalidateMeshInfo(){
	for(auto& m : meshInfos){
		delete m.first;
		m.first = nullptr;
	}
}

void RenderComponent::CreateMeshInstanceInfo(){
	meshInstanceInfo = App::GetRenderer().GenerateAPIMeshInstanceInfo(parent->GetTransformMatrix());
}

void RenderComponent::SetMaterial(StringID cachedMaterial_){
	for(auto& m : meshInfos){
		m.second = cachedMaterial_;
	}
}

void RenderComponent::SetMaterial(size_t index_, StringID cachedMaterial_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());
	if(meshInfos.size() <= index_){
		GADGET_LOG_WARNING(SID("RENDER"), "Invalid index for SetMaterial!");
		return;
	}

	meshInfos[index_].second = cachedMaterial_;
}

ComponentProperties RenderComponent::Serialize() const{
	ComponentProperties props = Component::Serialize();
	props.variables.Add(SID("ModelName"), modelName);
	GetCachedMaterial(0)->Serialize(props.variables); //TODO - Serialize multiple materials

	return props;
}

void RenderComponent::Deserialize(const ComponentProperties& props_){
	modelName = props_.variables.GetValue(SID("ModelName"), StringID::None).ToStr();
	const StringID materialType = props_.variables.GetValue(SID("MaterialType"), StringID::None).ToStr();
	const StringID shaderName = props_.variables.GetValue(SID("ShaderName"), StringID::None).ToStr();

	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(materialType != StringID::None);
	GADGET_BASIC_ASSERT(shaderName != StringID::None);

	CreateMeshInfo();
	CreateMeshInstanceInfo();

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

Material* RenderComponent::GetCachedMaterial(size_t meshIndex_) const{
	return GetCachedMaterial(meshInfos[meshIndex_].second);
}

Material* RenderComponent::GetCachedMaterial(StringID material_){
	Material* cachedMaterial = App::GetMaterialCache().GetMaterial(material_);
	GADGET_BASIC_ASSERT(cachedMaterial != nullptr);
	return cachedMaterial;
}