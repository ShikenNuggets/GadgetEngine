#include "Material.h"

#include "App.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

Material::Material(StringID shaderResource_) : shaderResourceName(shaderResource_), shader(nullptr){
	GADGET_BASIC_ASSERT(shaderResourceName != StringID::None);

	shader = App::GetRenderer().GenerateAPIShader(shaderResourceName);
	GADGET_ASSERT(shader != nullptr, "Could not load shader [" + shaderResourceName.GetString() + "]!");
}

Material::~Material(){
	//delete shader; //TODO - I don't know who's supposed to own the shader now but this causes a crash
	App::GetResourceManager().UnloadResource(shaderResourceName);
}