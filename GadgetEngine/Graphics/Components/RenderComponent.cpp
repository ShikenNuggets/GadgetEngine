#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

RenderComponent::RenderComponent(GameObject* parent_, StringID modelName_, StringID textureName_, StringID shaderName_) : Component(parent_), meshInfo(nullptr), material(nullptr){
	CreateMeshInfo(modelName_);
	material = new DiffuseTextureMaterial(textureName_, shaderName_);
}

RenderComponent::RenderComponent(GameObject* parent_, StringID modelName_, const Color& color_, StringID shaderName_) : Component(parent_), meshInfo(nullptr), material(nullptr){
	CreateMeshInfo(modelName_);
	material = new ColorMaterial(color_, shaderName_);
}

RenderComponent::RenderComponent(GameObject* parent_, StringID modelName_, Material* material_) : Component(parent_), meshInfo(nullptr), material(material_){
	CreateMeshInfo(modelName_);
}

RenderComponent::~RenderComponent(){
	delete material;
	delete meshInfo;
}

void RenderComponent::Bind(){
	meshInfo->Bind();
	material->Bind();
}

void RenderComponent::Unbind(){
	material->Unbind();
	meshInfo->Unbind();
}

void RenderComponent::CreateMeshInfo(StringID modelName_){
	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName_);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	meshInfo = App::GetRenderer().GenerateAPIMeshInfo(*mesh);
	GADGET_BASIC_ASSERT(meshInfo != nullptr);

	App::GetResourceManager().UnloadResource(modelName_);
}