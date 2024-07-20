#include "MaterialCache.h"

using namespace Gadget;

MaterialCache::MaterialCache() : materials(){}

MaterialCache::~MaterialCache(){
	for(const auto& m : materials){
		delete m;
	}
}

void MaterialCache::AddMaterial(StringID id_, Material* material_){
	GADGET_BASIC_ASSERT(id_ != StringID::None);
	GADGET_BASIC_ASSERT(material_ != nullptr);
	materials.Add(id_, material_);
}

Material* MaterialCache::GetMaterial(StringID id_) const{
	GADGET_BASIC_ASSERT(materials.Contains(id_));
	if(!materials.Contains(id_)){
		GADGET_LOG_WARNING(SID("RENDER"), "Tried to get invalid material ID [" + id_.GetString() + "]!");
		return nullptr;
	}

	return materials[id_];
}