#ifndef GADGET_ENGINE_MATERIAL_H
#define GADGET_ENGINE_MATERIAL_H

#include "Graphics/Color.h"
#include "Graphics/Materials/MaterialInfo.h"

namespace Gadget{
	class EngineMaterial{
	public:
		EngineMaterial(const Color& color_, bool materialInfoDeferred_ = true);
		EngineMaterial(const Color& colorA_, const Color& colorB_, const Color& colorC_, bool materialInfoDeferred_ = true);
		EngineMaterial(const std::vector<Color>& colors_, bool materialInfoDeferred_);
		~EngineMaterial();

		void CreateMaterialInfo();

		const std::vector<Color>& GetColors() const{ return colors; }
		MaterialInfo* GetMaterialInfo(){ return materialInfo; }

	private:
		std::vector<Color> colors;
		MaterialInfo* materialInfo;
	};
}

#endif //!GADGET_ENGINE_MATERIAL_H
