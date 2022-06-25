#ifndef GADGET_MATH_H
#define GADGET_MATH_H

namespace Gadget{
	class Math{
	public:
		static constexpr float Pi = 3.141592653589793238f;

		static inline constexpr float DegreesToRadians(const float angle_){
			return angle_ * (Pi / 180.0f);
		}

		static inline constexpr float RadiansToDegrees(const float angle_){
			return angle_ * (180.0f / Pi);
		}

		//Delete unwanted compiler-generated constructors, destructors, and assignment operators
		Math() = delete;
		Math(const Math&) = delete;
		Math(Math&&) = delete;
		Math& operator=(const Math&) = delete;
		Math& operator=(Math&&) = delete;
		~Math() = delete;
	};
}

#endif //!GADGET_MATH_H