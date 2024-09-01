#include "ActionTimer.h"

#include "Debug.h"
#include "Math/Math.h"

using namespace Gadget;

ActionTimer::ActionTimer(TimerType timerType_, ActionType actionType_, float duration_, const std::function<void(float)>& func_) : timerType(timerType_), actionType(actionType_), duration(duration_), func(func_), timer(0.0f){
	GADGET_BASIC_ASSERT(duration > 0.0f);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(duration));

	if(timerType == TimerType::Countdown){
		timer = duration_;
	}else{
		timer = 0.0f;
	}
}

bool ActionTimer::Update(float deltaTime_){
	if(timerType == TimerType::Countdown){
		return UpdateCountdown(deltaTime_);
	}else{
		return UpdateStopwatch(deltaTime_);
	}
}

bool ActionTimer::IsDone() const{
	if(timerType == TimerType::Countdown){
		return timer <= 0.0f;
	}else{
		return timer >= duration;
	}
}

bool ActionTimer::UpdateStopwatch(float deltaTime_){
	timer += deltaTime_;
	if(timer >= duration){
		func(timer / duration);
		return true;
	}

	if(actionType == ActionType::PerFrame){
		func(timer / duration);
	}

	return false;
}

bool ActionTimer::UpdateCountdown(float deltaTime_){
	timer -= deltaTime_;
	if(timer <= 0.0f){
		func(0.0f);
		return true;
	}

	if(actionType == ActionType::PerFrame){
		func(timer / duration);
	}

	return false;
}