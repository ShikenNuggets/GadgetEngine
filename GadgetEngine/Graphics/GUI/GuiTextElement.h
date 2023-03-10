#ifndef GADGET_GUI_TEXT_ELEMENT_H
#define GADGET_GUI_TEXT_ELEMENT_H

#include "GuiElement.h"
#include "Graphics/Text/Font.h"

namespace Gadget{
	class GuiTextElement : public GuiElement{
	public:
		GuiTextElement(StringID name_, const std::string& text_, StringID font_, const Vector2& pos_, GuiAnchor anchor_, bool isActive_ = true) : GuiElement(name_, pos_, anchor_, isActive_), text(text_), fontName(font_){}

	private:
		std::string text;
		StringID fontName;
	};
}

#endif //!GADGET_GUI_TEXT_ELEMENT_H