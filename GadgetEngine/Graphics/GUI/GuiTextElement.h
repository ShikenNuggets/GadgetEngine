#ifndef GADGET_GUI_TEXT_ELEMENT_H
#define GADGET_GUI_TEXT_ELEMENT_H

#include "GuiElement.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Text/TextMesh.h"

namespace Gadget{
	class GuiTextElement : public GuiElement{
	public:
		GuiTextElement(StringID name_, const std::string& text_, StringID font_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, Color color_ = Color::White(), bool isActive_ = true) : GuiElement(name_, pos_, size_, anchor_, isActive_), fontName(font_), textMesh(font_, SID("Text2DShader"), text_), color(color_){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(font_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ < GuiAnchor::GuiAnchor_MAX);
			GADGET_BASIC_ASSERT(color_.IsValid());
		}

		std::string GetText() const{ return textMesh.GetText(); }
		void SetText(const std::string& text_){ textMesh.SetText(text_); }

		const TextMesh& GetTextMesh() const{ return textMesh; }

		Color GetColor() const{ return color; }
		void SetColor(const Color& color_){ color = color_; }

	private:
		StringID fontName;
		TextMesh textMesh;
		Color color;
	};
}

#endif //!GADGET_GUI_TEXT_ELEMENT_H