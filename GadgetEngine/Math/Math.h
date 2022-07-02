#ifndef GADGET_MATH_H
#define GADGET_MATH_H

#include <limits>

namespace Gadget{
	class Math{
	public:
		static constexpr float Pi = 3.141592653589793238f;
		static constexpr float NearZero = std::numeric_limits<float>::denorm_min();
		static constexpr float Infinity = std::numeric_limits<float>::infinity();

		static inline constexpr float DegreesToRadians(const float angle_){
			return angle_ * (Pi / 180.0f);
		}

		static inline constexpr float RadiansToDegrees(const float angle_){
			return angle_ * (180.0f / Pi);
		}

		static inline float Sqrt(float value_){ return static_cast<float>(sqrt(value_)); }

		static inline constexpr bool IsNearZero(const float s){ return s <= NearZero && s >= -NearZero; }
		static inline constexpr bool Near(const float a, const float b){ return IsNearZero(a - b); }

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