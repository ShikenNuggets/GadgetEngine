#ifndef GADGET_MATH_H
#define GADGET_MATH_H

#include <limits>

#include "Debug.h"

namespace Gadget{
	class Math{
	public:
		static constexpr float Pi = 3.141592653589793238f;
		static constexpr float NearZero = std::numeric_limits<float>::denorm_min();
		static constexpr float Infinity = std::numeric_limits<float>::infinity();

		static inline constexpr float DegreesToRadians(float angle_){
			return angle_ * (Pi / 180.0f);
		}

		static inline constexpr float RadiansToDegrees(float angle_){
			return angle_ * (180.0f / Pi);
		}

		static inline float Sqrt(float value_){ return static_cast<float>(sqrt(value_)); }

		static inline constexpr bool IsNearZero(float s_){ return s_ <= NearZero && s_ >= -NearZero; }
		static inline constexpr bool Near(float a_, float b_){ return IsNearZero(a_ - b_); }

		static inline constexpr float SafeDivide(float a_, float b_){
			if(Math::IsNearZero(b_)){
				Debug::Log("Attempted to divide " + std::to_string(a_) + " by zero! Returning 0...", Debug::Warning);
				return 0;
			}

			return a_ / b_;
		}

		static inline constexpr float Dot2D(float aa_, float ab_, float ba_, float bb_){ return (aa_ * ba_) + (ab_ * bb_); }
		static inline constexpr float Dot3D(float aa_, float ab_, float ac_, float ba_, float bb_, float bc_){ return (aa_ * ba_) + (ab_ * bb_) + (ac_ * bc_); }
		static inline constexpr float Dot4D(float aa_, float ab_, float ac_, float ad_, float ba_, float bb_, float bc_, float bd_){ return (aa_ * ba_) + (ab_ * bb_) + (ac_ * bc_) + (ad_ * bd_); }

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