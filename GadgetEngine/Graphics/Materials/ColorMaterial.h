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
		ColorMaterial(const Color& color_, StringID shaderResource_) : Material(shaderResource_), color(color_){}

		virtual void Bind() override{
			shader->Bind();
			shader->BindColor(SID("material.color"), color);
		}

		virtual void Unbind() override{
			shader->Unbind();
		}

		virtual bool HasLighting() const override{ return false; }

	private:
		Color color;
	};
}

#endif //!GADGET_COLOR_MATERIAL_H