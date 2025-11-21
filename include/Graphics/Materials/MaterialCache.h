#ifndef GADGET_MATERIALS_MATERIAL_CACHE_H
#define GADGET_MATERIALS_MATERIAL_CACHE_H

#include "Data/HashTable.h"
#include "Graphics/Materials/Material.h"

namespace Gadget{
	class MaterialCache{
	public:
		MaterialCache();
		~MaterialCache();

		void AddMaterial(StringID id_, Material* material_);
		Material* GetMaterial(StringID id_) const;

		const HashTable<StringID, Material*>& GetMaterials() const{ return materials; }

	private:
		HashTable<StringID, Material*> materials;
	};
}

#endif //!GADGET_MATERIALS_MATERIAL_CACHE_H
