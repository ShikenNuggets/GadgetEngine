#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

ComponentCollection<RenderComponent> RenderComponent::componentCollection = ComponentCollection<RenderComponent>();

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, StringID textureName_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(textureName_ != StringID::None);
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo(modelName_);
	material = new DiffuseTextureMaterial(textureName_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, const Color& color_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(color_.IsValid());
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo(modelName_);
	material = new ColorMaterial(color_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, Material* material_) : Component(SID("RenderComponent"), parentGUID_), meshInfo(nullptr), material(material_){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);

	CreateMeshInfo(modelName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(const ComponentProperties& props_) : Component(props_), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	StringID modelName = props_.variables.GetValue(SID("ModelName")).ToStr();
	StringID materialType = props_.variables.GetValue(SID("MaterialType")).ToStr();
	StringID shaderName = props_.variables.GetValue(SID("ShaderName")).ToStr();

	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(materialType != StringID::None);
	GADGET_BASIC_ASSERT(shaderName != StringID::None);

	CreateMeshInfo(modelName);

	if(materialType == SID("DiffuseTextureMaterial")){
		StringID textureName = props_.variables.GetValue(SID("TextureName")).ToStr();
		
		GADGET_BASIC_ASSERT(textureName != StringID::None);
		
		material = new DiffuseTextureMaterial(textureName, shaderName);
	}else if(materialType == SID("ColorMaterial")){
		float colorR = props_.variables.GetValue(SID("Color_R")).ToNumber<float>();
		float colorG = props_.variables.GetValue(SID("Color_R")).ToNumber<float>();
		float colorB = props_.variables.GetValue(SID("Color_R")).ToNumber<float>();
		float colorA = props_.variables.GetValue(SID("Color_R")).ToNumber<float>();

		GADGET_BASIC_ASSERT(Math::IsValidNumber(colorR));
		GADGET_BASIC_ASSERT(Math::IsValidNumber(colorG));
		GADGET_BASIC_ASSERT(Math::IsValidNumber(colorB));
		GADGET_BASIC_ASSERT(Math::IsValidNumber(colorA));

		material = new ColorMaterial(Color(colorR, colorG, colorB, colorA), shaderName);
	}

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::~RenderComponent(){
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);

	delete material;
	delete meshInfo;

	componentCollection.Remove(this);

	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == nullptr);
}

void RenderComponent::Bind(){
	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);

	meshInfo->Bind();
	material->Bind();
}

void RenderComponent::Unbind(){
	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);

	material->Unbind();
	meshInfo->Unbind();
}

void RenderComponent::CreateMeshInfo(StringID modelName_){
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);

	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName_);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	meshInfo = App::GetRenderer().GenerateAPIMeshInfo(*mesh);
	GADGET_BASIC_ASSERT(meshInfo != nullptr);

	App::GetResourceManager().UnloadResource(modelName_);
}