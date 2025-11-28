#ifndef GADGET_FPS_DISPLAY_ELEMENT_H
#define GADGET_FPS_DISPLAY_ELEMENT_H

#include "GuiTextElement.h"

namespace Gadget{
	class FpsDisplayElement : public GuiTextElement{
	public:
		FpsDisplayElement(StringID name_, StringID font_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : GuiTextElement(name_, "", font_, pos_, size_, anchor_, Color::White(), isActive_), frameCount(0), timer(0.0){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(font_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ < GuiAnchor::GuiAnchor_MAX);
		}
		
		virtual void Update([[maybe_unused]] float deltaTime_) override{
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_) && deltaTime_ >= 0.0f);
			
			frameCount++;
			timer += deltaTime_;

			if(timer < 1.0){
				return;
			}

			const double total = frameCount / timer;

			std::stringstream stream;
			stream << std::fixed << std::setprecision(0) << total;

			SetText(stream.str() + " FPS");

			frameCount = 0;
			timer -= 1.0f;
		}

	private:
		int64_t frameCount;
		double timer;
	};
}

#endif //!GADGET_FPS_DISPLAY_ELEMENT_H