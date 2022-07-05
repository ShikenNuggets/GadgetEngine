#ifndef GADGET_MATH_H
#define GADGET_MATH_H

#include <limits>

#include "Debug.h"
#include "Math/Angle.h"

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

		static inline float Abs(float value_){ return abs(value_); }

		static inline constexpr float Dot2D(float aa_, float ab_, float ba_, float bb_){ return (aa_ * ba_) + (ab_ * bb_); }
		static inline constexpr float Dot3D(float aa_, float ab_, float ac_, float ba_, float bb_, float bc_){ return (aa_ * ba_) + (ab_ * bb_) + (ac_ * bc_); }
		static inline constexpr float Dot4D(float aa_, float ab_, float ac_, float ad_, float ba_, float bb_, float bc_, float bd_){ return (aa_ * ba_) + (ab_ * bb_) + (ac_ * bc_) + (ad_ * bd_); }

		template <class T>
		static inline constexpr T Clamp(T min_, T max_, T value_){
			if(value_ < min_){
				return min_;
			}else if(value_ > max_){
				return max_;
			}

			return value_;
		}

		static inline float Sin(Angle angle_){ return SinR(angle_.ToRadians()); }
		static inline float Cos(Angle angle_){ return CosR(angle_.ToRadians()); }
		static inline float Tan(Angle angle_){ return TanR(angle_.ToRadians()); }

		static inline float SinR(Radian radian_){ return static_cast<float>(sin(radian_.Get())); }
		static inline float CosR(Radian radian_){ return static_cast<float>(cos(radian_.Get())); }
		static inline float TanR(Radian radian_){ return static_cast<float>(tan(radian_.Get())); }

		static inline Angle Asin(float sin_){ return Radian(static_cast<float>(asin(sin_))).ToDegrees(); }
		static inline Angle Acos(float cos_){ return Radian(static_cast<float>(acos(Clamp(-1.0f, 1.0f, cos_)))).ToDegrees(); }
		static inline Angle Atan(float tan_){ return Radian(static_cast<float>(atan(tan_))).ToDegrees(); }
		static inline Angle Atan2(float a_, float b_){ return Radian(static_cast<float>(atan2(a_, b_))).ToDegrees(); }

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