#include "MaterialCache.h"

#include "Graphics/Materials/ColorMaterial.h"

using namespace Gadget;

MaterialCache::MaterialCache() : materials(){
#ifdef GADGET_DEBUG
	materials.Add(SID("Invalid"), new ColorMaterial(Gadget::Color::Pink(), SID("ColorShader")));
#else
	materials.Add(SID("Invalid"), new ColorMaterial(Gadget::Color::Black(), SID("ColorShader")));
#endif //GADGET_DEBUG
}

MaterialCache::~MaterialCache(){
	for(auto& [key, value] : materials){
		delete value;
	}
}

void MaterialCache::AddMaterial(StringID id_, Material* material_){
	GADGET_BASIC_ASSERT(id_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);
	materials.Add(id_, material_);
}

Material* MaterialCache::GetMaterial(StringID id_) const{
	GADGET_SLOW_ASSERT(materials.Contains(id_));

	auto* material = materials.Find(id_);
	if(material == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "Tried to get invalid material ID [" + id_.GetString() + "]! Using placeholder material");
		return materials[SID("Invalid")];
	}

	return *material;
}