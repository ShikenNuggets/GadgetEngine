#include "RenderComponent.h"

#include "App.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

RenderComponent::RenderComponent(GameObject* parent_, StringID modelName_, StringID textureName_, StringID shaderName_) : Component(parent_), meshInfo(nullptr), material(nullptr){
	Mesh* mesh = App::GetResourceManager().LoadResource<Mesh>(modelName_);
	GADGET_BASIC_ASSERT(mesh != nullptr);

	meshInfo = App::GetRenderer().GenerateAPIMeshInfo(*mesh);
	GADGET_BASIC_ASSERT(meshInfo != nullptr);

	App::GetResourceManager().UnloadResource(modelName_);

	material = new DiffuseTextureMaterial(textureName_, shaderName_);
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