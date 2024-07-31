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
		static const StringID type;

		ColorMaterial(const Color& color_, StringID shaderResource_) : Material(shaderResource_), color(color_){
			GADGET_BASIC_ASSERT(color_.IsValid());
			GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);
			GADGET_BASIC_ASSERT(shader != nullptr);
		}

		ColorMaterial(const NamedVarList& varList_) : Material(varList_), color(Color::Black()){
			ColorMaterial::Deserialize(varList_);
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

		Color GetColor() const{ return color; }
		virtual bool HasLighting() const override{ return false; }

		void SetColor(const Color& color_){
			color = color_;
			GADGET_BASIC_ASSERT(color_.IsValid());
		}

		virtual StringID Type() const override{ return type; }

		virtual void Serialize(NamedVarList& varList_) const override{
			Material::Serialize(varList_);
			varList_.Add(SID("Color_R"), color.r);
			varList_.Add(SID("Color_G"), color.g);
			varList_.Add(SID("Color_B"), color.b);
			varList_.Add(SID("Color_A"), color.a);
		}

	protected:
		virtual void Deserialize(const NamedVarList& varList_) override{
			GADGET_BASIC_ASSERT(ColorMaterial::Type() == varList_.GetValue(SID("MaterialType"), StringID::None).ToStr());
			color.r = varList_.GetValue(SID("Color_R"), Color::White().r).ToNumber<float>();
			color.g = varList_.GetValue(SID("Color_G"), Color::White().g).ToNumber<float>();
			color.b = varList_.GetValue(SID("Color_B"), Color::White().b).ToNumber<float>();
			color.a = varList_.GetValue(SID("Color_A"), Color::White().a).ToNumber<float>();
		}

	private:
		Color color;
	};
}

#endif //!GADGET_COLOR_MATERIAL_H