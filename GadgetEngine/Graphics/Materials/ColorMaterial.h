#ifndef GADGET_COLOR_MATERIAL_H
#define GADGET_COLOR_MATERIAL_H

#include "Material.h"
#include "App.h"
#include "Graphics/TextureInfo.h"
#include "Resource/ResourceManager.h"
#include "Utils/StringID.h"

namespace Gadget{
	class ColorMaterial : public Material{
	public:
		ColorMaterial(const Color& color_, StringID shaderResource_) : Material(shaderResource_), color(color_){
			GADGET_BASIC_ASSERT(color_.IsValid());
			GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);
			GADGET_BASIC_ASSERT(shader != nullptr);
		}

		virtual void Bind() override{
			GADGET_BASIC_ASSERT(shader != nullptr);

			shader->Bind();
			shader->BindColor(SID("material.color"), color);
		}

		virtual void Unbind() override{
			GADGET_BASIC_ASSERT(shader != nullptr);

			shader->Unbind();
		}

		virtual bool HasLighting() const override{ return false; }

	private:
		Color color;
	};
}

#endif //!GADGET_COLOR_MATERIAL_H