#include "Material.h"

#include "ColorMaterial.h"
#include "DiffuseTextureMaterial.h"
#include "App.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

const StringID ColorMaterial::type = SID("ColorMaterial");
const StringID DiffuseTextureMaterial::type = SID("DiffuseTextureMaterial");

Material::Material(StringID shaderResource_) : shaderResourceName(shaderResource_), shader(nullptr){
	GADGET_BASIC_ASSERT(shaderResourceName != StringID::None);

	shader = App::GetRenderer().GenerateAPIShader(shaderResourceName);
	GADGET_ASSERT(shader != nullptr, "Could not load shader [" + shaderResourceName.GetString() + "]!");
}

Material::Material(const NamedVarList& varList_) : shaderResourceName(StringID::None){
	Deserialize(varList_);
}

Material::~Material(){
	//delete shader; //TODO - I don't know who's supposed to own the shader now but this causes a crash
	App::GetResourceManager().UnloadResource(shaderResourceName);
}

void Material::Serialize(NamedVarList& varList_) const{
	varList_.Add(SID("MaterialType"), Type());
	varList_.Add(SID("ShaderName"), shaderResourceName);
}

void Material::Deserialize(const NamedVarList& varList_){
	GADGET_BASIC_ASSERT(Type() == varList_.GetValue(SID("MaterialType")).ToStr());
	shaderResourceName = varList_.GetValue(SID("ShaderName")).ToStr();
}