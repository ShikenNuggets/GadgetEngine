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

	LoadShader();
}

Material::Material(const NamedVarList& varList_) : shaderResourceName(StringID::None), shader(nullptr){
	Material::Deserialize(varList_);
}

Material::~Material(){
	InvalidateShader();
}

void Material::Serialize(NamedVarList& varList_) const{
	varList_.Add(SID("MaterialType"), Type());
	varList_.Add(SID("ShaderName"), shaderResourceName);
}

void Material::LoadShader(){
	InvalidateShader();
	shader = App::GetRenderer().GenerateAPIShader(shaderResourceName);
	GADGET_ASSERT(shader != nullptr, "Could not load shader [" + shaderResourceName.GetString() + "]!");
}

void Material::InvalidateShader(){
	App::GetResourceManager().UnloadResource(shaderResourceName);
	shader = nullptr;
}

void Material::Deserialize(const NamedVarList& varList_){
	shaderResourceName = varList_.GetValue(SID("ShaderName"), StringID::None).ToStr();
}