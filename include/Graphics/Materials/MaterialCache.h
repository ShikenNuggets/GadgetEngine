#ifndef GADGET_MATERIALS_MATERIAL_CACHE_H
#define GADGET_MATERIALS_MATERIAL_CACHE_H

#include <unordered_map>

#include "Graphics/Materials/Material.h"

namespace Gadget{
	class MaterialCache{
	public:
		MaterialCache();
		~MaterialCache();

		void AddMaterial(StringID id_, Material* material_);
		Material* GetMaterial(StringID id_) const;

		const std::unordered_map<StringID, Material*>& GetMaterials() const{ return materials; }

	private:
		std::unordered_map<StringID, Material*> materials;
	};
}

#endif //!GADGET_MATERIALS_MATERIAL_CACHE_H
