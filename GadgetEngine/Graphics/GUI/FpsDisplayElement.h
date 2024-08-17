#ifndef GADGET_FPS_DISPLAY_ELEMENT_H
#define GADGET_FPS_DISPLAY_ELEMENT_H

#include "GuiTextElement.h"

namespace Gadget{
	class FpsDisplayElement : public GuiTextElement{
	public:
		FpsDisplayElement(StringID name_, StringID font_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : GuiTextElement(name_, "", font_, pos_, size_, anchor_, Color::White(), isActive_), timeCounter(0.0f), previousFrameTimes(){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(font_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ < GuiAnchor::GuiAnchor_MAX);
		}
		
		virtual void Update([[maybe_unused]] float deltaTime_) override{
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_) && deltaTime_ >= 0.0f);

			timeCounter += App::GetTime().PureDeltaTime();
			previousFrameTimes.push_back(App::GetTime().PureDeltaTime());

			while(previousFrameTimes.size() > 60){
				previousFrameTimes.erase(previousFrameTimes.begin());
			}

			if(previousFrameTimes.size() < 60 || timeCounter < 1.0f){
				return;
			}

			float total = 0.0f;
			for(const auto& t : previousFrameTimes){
				total += t;
			}
			total /= previousFrameTimes.size();
			total *= 1000.0f;
			total = 1000.0f / total;

			std::stringstream stream;
			stream << std::fixed << std::setprecision(0) << total;

			SetText(stream.str() + " FPS");

			timeCounter -= 1.0f;
		}

	private:
		float timeCounter;
		std::vector<float> previousFrameTimes;
	};
}

#endif //!GADGET_FPS_DISPLAY_ELEMENT_H