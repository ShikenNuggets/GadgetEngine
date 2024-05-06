#ifndef GADGET_ENGINE_MATERIAL_H
#define GADGET_ENGINE_MATERIAL_H

#include "Graphics/Color.h"
#include "Graphics/Materials/MaterialInfo.h"

namespace Gadget{
	class EngineMaterial{
	public:
		EngineMaterial(const Color& color_);
		EngineMaterial(const Color& colorA_, const Color& colorB_, const Color& colorC_);
		EngineMaterial(const std::vector<Color>& color_);
		~EngineMaterial();

	private:
		std::vector<Color> colors;
		MaterialInfo* materialInfo;
	};
}

#endif //!GADGET_ENGINE_MATERIAL_H
