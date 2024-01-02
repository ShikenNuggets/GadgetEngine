#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

ComponentCollection<RenderComponent> RenderComponent::componentCollection = ComponentCollection<RenderComponent>();

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, StringID textureName_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(textureName_ != StringID::None);
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo();
	material = new DiffuseTextureMaterial(textureName_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, const Color& color_, StringID shaderName_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(color_.IsValid());
	GADGET_BASIC_ASSERT(shaderName_ != StringID::None);
	
	CreateMeshInfo();
	material = new ColorMaterial(color_, shaderName_);

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(GUID parentGUID_, StringID modelName_, Material* material_) : Component(SID("RenderComponent"), parentGUID_), modelName(modelName_), meshInfo(nullptr), material(material_){
	GADGET_BASIC_ASSERT(parentGUID_ != GUID::Invalid);
	GADGET_BASIC_ASSERT(modelName_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);

	CreateMeshInfo();

	componentCollection.Add(this);

	GADGET_BASIC_ASSERT(meshInfo != nullptr);
	GADGET_BASIC_ASSERT(material != nullptr);
	GADGET_BASIC_ASSERT(componentCollection.Get(parent->GetGUID()) == this);
}

RenderComponent::RenderComponent(const ComponentProperties& props_) : Component(props_), modelName(StringID::None), meshInfo(nullptr), material(nullptr){
	GADGET_BASIC_ASSERT(props_.parentGuid != GUID::Invalid);

	Deserialize(props_);

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

void RenderComponent::CreateMeshInfo(){
	GADGET_BASIC_ASSERT(modelName != StringID::None);

	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	meshInfo = App::GetRenderer().GenerateAPIMeshInfo(*mesh);
	GADGET_BASIC_ASSERT(meshInfo != nullptr);

	App::GetResourceManager().UnloadResource(modelName);
}

ComponentProperties RenderComponent::Serialize() const{
	ComponentProperties props = Component::Serialize();
	props.variables.Add(SID("ModelName"), modelName);
	props.variables.Add(SID("MaterialType"), material->Type());
	props.variables.Add(SID("ShaderName"), material->GetShaderName());

	//TODO - Maybe materials should serialize their own darn properties!
	if(material->Type() == DiffuseTextureMaterial::type){
		DiffuseTextureMaterial* dtmat = dynamic_cast<DiffuseTextureMaterial*>(material);
		props.variables.Add(SID("TextureName"), dtmat->TextureResourceName());
	}else if(material->Type() == ColorMaterial::type){
		ColorMaterial* cmat = dynamic_cast<ColorMaterial*>(material);
		props.variables.Add(SID("Color_R"), cmat->GetColor().r);
		props.variables.Add(SID("Color_G"), cmat->GetColor().g);
		props.variables.Add(SID("Color_B"), cmat->GetColor().b);
		props.variables.Add(SID("Color_A"), cmat->GetColor().a);
	}

	return props;
}

void RenderComponent::Deserialize(const ComponentProperties& props_){
	modelName = props_.variables.GetValue(SID("ModelName")).ToStr();
	StringID materialType = props_.variables.GetValue(SID("MaterialType")).ToStr();
	StringID shaderName = props_.variables.GetValue(SID("ShaderName")).ToStr();

	GADGET_BASIC_ASSERT(modelName != StringID::None);
	GADGET_BASIC_ASSERT(materialType != StringID::None);
	GADGET_BASIC_ASSERT(shaderName != StringID::None);

	CreateMeshInfo();

	if(materialType == DiffuseTextureMaterial::type){
		StringID textureName = props_.variables.GetValue(SID("TextureName")).ToStr();

		GADGET_BASIC_ASSERT(textureName != StringID::None);

		material = new DiffuseTextureMaterial(textureName, shaderName);
	}else if(materialType == ColorMaterial::type){
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
}