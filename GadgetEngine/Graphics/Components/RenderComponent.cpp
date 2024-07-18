#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

ComponentCollection<RenderComponent> RenderComponent::componentCollection = ComponentCollection<RenderComponent>();

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, StringID textureName_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), material(nullptr), engineMaterial(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(textureName_ != StringID::None);
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo();
	CreateMeshInstanceInfo();
	material = new DiffuseTextureMaterial(textureName_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, const Color& color_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), material(nullptr), engineMaterial(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(color_.IsValid());
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo();
	CreateMeshInstanceInfo();
	material = new ColorMaterial(color_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, Material* material_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), material(material_), engineMaterial(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);

	CreateMeshInfo();
	CreateMeshInstanceInfo();

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, EngineMaterial* material_, bool setMeshInfoDeferred_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfos(), material(nullptr), engineMaterial(material_){
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

RenderComponent::RenderComponent(const ComponentProperties& props_) : Component(props_), modelName(StringID::None), meshInfos(), material(nullptr){
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	Deserialize(props_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(!meshInfos.empty());
	GADGET_BASIC_ASSERT(meshInstanceInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::~RenderComponent(){
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

	delete engineMaterial;
	delete material;
	delete meshInstanceInfo;

	for(const auto& m : meshInfos){
		delete m;
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
	GADGET_BASIC_ASSERT(material != nullptr || engineMaterial != nullptr);

	meshInfos[index_]->Bind();

	if(material != nullptr){
		material->Bind();
	}
}

void RenderComponent::Unbind(size_t index_){
	GADGET_BASIC_ASSERT(index_ < meshInfos.size());
	GADGET_BASIC_ASSERT(material != nullptr || engineMaterial != nullptr);

	if(material != nullptr){
		material->Unbind();
	}

	meshInfos[index_]->Unbind();
}

void RenderComponent::CreateMeshInfo(){
	GADGET_BASIC_ASSERT(modelName != StringID::None);

	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	meshInfos = App::GetRenderer().GenerateAPIMeshInfos(*mesh);
	GADGET_BASIC_ASSERT(!meshInfos.empty());

	App::GetResourceManager().UnloadResource(modelName);
}

void RenderComponent::CreateMeshInstanceInfo(){
	meshInstanceInfo = App::GetRenderer().GenerateAPIMeshInstanceInfo(parent->GetTransformMatrix());
}

ComponentProperties RenderComponent::Serialize() const{
	ComponentProperties props = Component::Serialize();
	props.variables.Add(SID("ModelName"), modelName);
	material->Serialize(props.variables);

	return props;
}

void RenderComponent::Deserialize(const ComponentProperties& props_){
	modelName = props_.variables.GetValue(SID("ModelName"), StringID::None).ToStr();
	StringID materialType = props_.variables.GetValue(SID("MaterialType"), StringID::None).ToStr();
	StringID shaderName = props_.variables.GetValue(SID("ShaderName"), StringID::None).ToStr();

	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(materialType != StringID::None);
	GADGET_BASIC_ASSERT(shaderName != StringID::None);

	CreateMeshInfo();
	CreateMeshInstanceInfo();

	if(materialType == DiffuseTextureMaterial::type){
		material = new DiffuseTextureMaterial(props_.variables);
	}else if(materialType == ColorMaterial::type){
		material = new ColorMaterial(props_.variables);
	}else{
		GADGET_ASSERT_NOT_IMPLEMENTED;
	}
}