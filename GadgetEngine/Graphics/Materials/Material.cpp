#include "Material.h"

#include "App.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

Material::Material(StringID shaderResource_) : shaderResourceName(shaderResource_), shader(nullptr){
	shader = App::GetInstance()->GetRenderer()->GenerateAPIShader(shaderResourceName);
	GADGET_ASSERT(shader != nullptr, "Could not load shader [" + shaderResourceName.GetString() + "]!");
}

Material::~Material(){
	delete shader;
	ResourceManager::GetInstance()->UnloadResource(shaderResourceName);
}