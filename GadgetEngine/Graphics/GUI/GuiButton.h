#ifndef GADGET_GUI_BUTTON_H
#define GADGET_GUI_BUTTON_H

#include <functional>

#include "Utils/StringID.h"
#include "GuiTextElement.h"
#include "GuiTextureElement.h"

namespace Gadget{
	class GuiButton : public GuiElement{
	public:
		GuiButton(StringID name_, const std::string& text_, StringID font_, StringID texture_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : GuiElement(name_, pos_, size_, anchor_, isActive_), text(nullptr), texture(nullptr){
			text = new GuiTextElement(SID("GuiButton_text"), text_, font_, pos_, size_, anchor_, isActive_);
			texture = new GuiTextureElement(SID("GuiButton_texture"), texture_, pos_, size_, anchor_, isActive_);

			AddSubElement(text);
			AddSubElement(texture);
		}

		GuiTextElement* GetTextElement(){ return text; }
		GuiTextureElement* GetTextureElement(){ return texture; }

		void SetOnClickCallback(const std::function<void(void)>& func_){
			onClickFunc = func_;
		}

		virtual void OnClick(ButtonID button_, const Vector2& clickPoint_) override{
			if(button_ == ButtonID::Mouse_LeftMouseButton){
				onClickFunc();
			}
		}

	private:
		GuiTextElement* text;
		GuiTextureElement* texture;
		std::function<void(void)> onClickFunc;
	};
}

#endif //!GADGET_GUI_BUTTON_H