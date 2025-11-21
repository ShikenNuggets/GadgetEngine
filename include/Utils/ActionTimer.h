#ifndef GADGET_UTILS_ACTION_TIMER_H
#define GADGET_UTILS_ACTION_TIMER_H

#include <functional>

namespace Gadget{
	class ActionTimer{
	public:
		enum class TimerType{
			Stopwatch,
			Countdown
		};

		enum class ActionType{
			OnEnd,
			PerFrame
		};

		ActionTimer(TimerType timerType_, ActionType actionType_, float duration_, const std::function<void(float)>& func_);

		bool Update(float deltaTime_);
		bool IsDone() const;

	private:
		TimerType timerType;
		ActionType actionType;
		float duration;
		std::function<void(float)> func;

		float timer;

		bool UpdateStopwatch(float deltaTime_);
		bool UpdateCountdown(float deltaTime_);
	};
}

#endif //!GADGET_UTILS_ACTION_TIMER_H