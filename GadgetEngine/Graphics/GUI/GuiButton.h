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
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(font_ != StringID::None);
			GADGET_BASIC_ASSERT(texture_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ != GuiAnchor::GuiAnchor_MAX);
			
			text = new GuiTextElement(SID("GuiButton_text"), text_, font_, pos_, size_, anchor_, Color::White(), isActive_);
			texture = new GuiTextureElement(SID("GuiButton_texture"), texture_, pos_, size_, anchor_, Color::White(), isActive_);

			AddSubElement(text);
			AddSubElement(texture);

			GADGET_BASIC_ASSERT(text != nullptr);
			GADGET_BASIC_ASSERT(texture != nullptr);
			GADGET_BASIC_ASSERT(Utils::Contains(subElements, dynamic_cast<GuiElement*>(text)));
		}

		GuiTextElement* GetTextElement(){ return text; }
		GuiTextureElement* GetTextureElement(){ return texture; }

		void SetOnClickCallback(const std::function<void(ButtonID, const Vector2&)>& func_){
			onClickFunc = func_;
		}

		virtual void OnClick(ButtonID button_, const Vector2& clickPoint_) override{
			GADGET_BASIC_ASSERT(button_ < ButtonID::ButtonID_MAX);
			GADGET_BASIC_ASSERT(clickPoint_.IsValid());

			if(IsActive() && button_ == ButtonID::Mouse_LeftMouseButton && onClickFunc){
				onClickFunc(button_, clickPoint_);
			}
		}

	private:
		GuiTextElement* text;
		GuiTextureElement* texture;
		std::function<void(ButtonID, const Vector2&)> onClickFunc;
	};
}

#endif //!GADGET_GUI_BUTTON_H