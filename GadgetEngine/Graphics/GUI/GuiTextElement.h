#ifndef GADGET_GUI_TEXT_ELEMENT_H
#define GADGET_GUI_TEXT_ELEMENT_H

#include "GuiElement.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Text/TextMesh.h"

namespace Gadget{
	class GuiTextElement : public GuiElement{
	public:
		GuiTextElement(StringID name_, const std::string& text_, StringID font_, const Vector2& pos_, GuiAnchor anchor_, bool isActive_ = true) : GuiElement(name_, pos_, anchor_, isActive_), text(text_), fontName(font_), textMesh(font_, text_){}

		std::string GetText() const{ return text; }
		void SetText(const std::string& text_){ text = text_; }

		const TextMesh& GetTextMesh() const{ return textMesh; }

	private:
		std::string text;
		StringID fontName;

		TextMesh textMesh;
	};
}

#endif //!GADGET_GUI_TEXT_ELEMENT_H