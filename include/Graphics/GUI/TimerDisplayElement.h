#ifndef GADGET_GRAPHICS_GUI_TIMER_DISPLAY_ELEMENT_H
#define GADGET_GRAPHICS_GUI_TIMER_DISPLAY_ELEMENT_H

#include "GuiTextElement.h"

namespace Gadget{
	class TimerDisplayElement : public GuiTextElement{
	public:
		enum class Type : uint8_t{
			TimerToZero = 0,
			StopWatch,

			Type_MAX
		};

		TimerDisplayElement(StringID name_, StringID font_, Type type_, float targetTime_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : GuiTextElement(name_, "", font_, pos_, size_, anchor_, Color::White(), isActive_), type(type_), targetTime(targetTime_), timer(0.0f), isPaused(false){
			GADGET_BASIC_ASSERT(name_ != StringID::None);
			GADGET_BASIC_ASSERT(font_ != StringID::None);
			GADGET_BASIC_ASSERT(type_ < Type::Type_MAX);
			GADGET_BASIC_ASSERT(Math::IsValidNumber(targetTime_));
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ < GuiAnchor::GuiAnchor_MAX);

			if(type == Type::TimerToZero){
				timer = targetTime;
			}
		}

		virtual void Update([[maybe_unused]] float deltaTime_) override{
			GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_));
			GuiTextElement::Update(deltaTime_);

			if(!isPaused){
				switch(type){
					case Type::TimerToZero:
						timer -= deltaTime_;
						if(timer < 0.0f){
							timer = 0.0f;
						}
						break;
					case Type::StopWatch:
						timer += deltaTime_;
						if(timer > targetTime){
							timer = targetTime;
						}
						break;
					default:
						GADGET_ASSERT_NOT_IMPLEMENTED;
						break;
				}
			}

			SetText(Utils::FloatSecondsToTimeString(std::trunc(timer)));
		}

		float GetTimer() const{ return timer; }

		bool IsDone() const{
			if(type == Type::TimerToZero){
				return timer <= 0.0f;
			}else{
				return timer >= targetTime;
			}
		}

		void Pause(bool pause_ = true){ isPaused = pause_; }

		void Reset(){
			switch(type){
				case Type::TimerToZero:
					timer = targetTime;
					break;
				case Type::StopWatch:
					timer = 0.0f;
					break;
				default:
					GADGET_ASSERT_NOT_IMPLEMENTED;
					break;
			}
		}

	private:
		Type type;
		const float targetTime;
		float timer;
		bool isPaused;
	};
}

#endif //!GADGET_GRAPHICS_GUI_TIMER_DISPLAY_ELEMENT_H