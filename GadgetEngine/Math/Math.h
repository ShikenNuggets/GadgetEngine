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
		static constexpr uint64_t LargestPrime = 18'446'744'073'709'551'557; //Largest prime number that can be represented by a 64 bit integer

		static inline bool IsValidNumber(float value_){
			return !std::isnan(value_) && !std::isinf(value_);
		}

		static inline bool IsValidNumber(double value_){
			return !isnan(value_) && !isinf(value_);
		}

		static inline constexpr float DegreesToRadians(float angle_){ return angle_ * (Pi / 180.0f); }

		static inline constexpr float RadiansToDegrees(float angle_){ return angle_ * (180.0f / Pi); }

		static inline float Sqrt(float value_){
			GADGET_BASIC_ASSERT(IsValidNumber(value_));
			GADGET_BASIC_ASSERT(IsValidNumber(sqrt(value_)));

			return static_cast<float>(sqrt(value_));
		}

		static inline constexpr bool IsNearZero(float s_){ return s_ <= NearZero && s_ >= -NearZero; }
		static inline constexpr bool IsNearZero(double s_){ return s_ <= static_cast<double>(NearZero) && s_ >= static_cast<double>(-NearZero); }
		static inline constexpr bool Near(float a_, float b_){ return IsNearZero(a_ - b_); }
		static inline constexpr bool Near(double a_, double b_){ return IsNearZero(a_ - b_); }

		static inline constexpr float SafeDivide(float a_, float b_){
			if(Math::IsNearZero(b_)){
				Debug::Log("Attempted to divide " + std::to_string(a_) + " by zero! Returning 0...", Debug::Warning);
				return 0;
			}

			return a_ / b_;
		}

		template <class T>
		static inline constexpr T Abs(T value_){
			if(value_ < 0){
				return value_ * -1;
			}

			return value_;
		}

		static inline float Abs(float value_){
			GADGET_BASIC_ASSERT(IsValidNumber(value_));
			return Abs<float>(value_);
		}

		static inline double Abs(double value_){
			GADGET_BASIC_ASSERT(IsValidNumber(value_));
			return Abs<double>(value_);
		}

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

		static inline float Sin(Angle angle_){
			GADGET_BASIC_ASSERT(IsValidNumber(angle_.Get()));
			return SinR(angle_.ToRadians());
		}

		static inline float Cos(Angle angle_){
			GADGET_BASIC_ASSERT(IsValidNumber(angle_.Get()));
			return CosR(angle_.ToRadians());
		}

		static inline float Tan(Angle angle_){
			GADGET_BASIC_ASSERT(IsValidNumber(angle_.Get()));
			return TanR(angle_.ToRadians());
		}

		static inline float SinR(Radian radian_){
			GADGET_BASIC_ASSERT(IsValidNumber(radian_.Get()));
			return static_cast<float>(sin(radian_.Get()));
		}

		static inline float CosR(Radian radian_){
			GADGET_BASIC_ASSERT(IsValidNumber(radian_.Get()));
			return static_cast<float>(cos(radian_.Get()));
		}

		static inline float TanR(Radian radian_){
			GADGET_BASIC_ASSERT(IsValidNumber(radian_.Get()));
			return static_cast<float>(tan(radian_.Get()));
		}

		static inline Angle Asin(float sin_){
			GADGET_BASIC_ASSERT(IsValidNumber(sin_));
			return Radian(static_cast<float>(asin(sin_))).ToDegrees();
		}

		static inline Angle Acos(float cos_){
			GADGET_BASIC_ASSERT(IsValidNumber(cos_));
			return Radian(static_cast<float>(acos(Clamp(-1.0f, 1.0f, cos_)))).ToDegrees();
		}

		static inline Angle Atan(float tan_){
			GADGET_BASIC_ASSERT(IsValidNumber(tan_));
			return Radian(static_cast<float>(atan(tan_))).ToDegrees();
		}

		static inline Angle Atan2(float a_, float b_){
			GADGET_BASIC_ASSERT(IsValidNumber(a_));
			GADGET_BASIC_ASSERT(IsValidNumber(b_));
			return Radian(static_cast<float>(atan2(a_, b_))).ToDegrees();
		}

		static inline bool IsPrime(uint64_t num_){
			GADGET_ASSERT(num_ <= LargestPrime, "Checking IsPrime on an extremely large non-prime");

			if(num_ == 0 || num_ == 1 || num_ > LargestPrime){
				return false;
			}

			float sqrtNum = Sqrt(static_cast<float>(num_));

			for(uint64_t i = 2; i <= sqrtNum; i++){
				if(num_ % i == 0){
					return false;
				}
			}

			return true;
		}

		static inline uint64_t NextPrime(uint64_t start){
			GADGET_ASSERT(start <= LargestPrime, "Trying to get NextPrime, but [" + std::to_string(start) + "] is already larger than the largest available prime!");
			if(start >= LargestPrime){
				return 0;
			}

			for(uint64_t i = start + 1; i <= std::numeric_limits<uint64_t>::max(); i++){
				if(IsPrime(i)){
					return i;
				}
			}

			GADGET_ASSERT(false, "Something went wrong trying to get the next prime number from [" + std::to_string(start) + "]!");
			return 0;
		}

		static inline constexpr bool IsInteger(double num_){ return Math::Near(static_cast<double>(static_cast<int64_t>(num_)), num_); }

		static inline float Floor(float num_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(num_));
			return floor(num_);
		}

		static inline double Floor(double num_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(num_));
			return floor(num_);
		}

		static inline float Ceiling(float num_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(num_));
			return ceil(num_);
		}

		static inline double Ceiling(double num_){
			GADGET_BASIC_ASSERT(Math::IsValidNumber(num_));
			return ceil(num_);
		}

		static inline constexpr float RemapRange(float value_, float oldMin_, float oldMax_, float newMin_, float newMax_){
			if(oldMin_ == oldMax_ || newMin_ == newMax_){
				return newMin_;
			}

			float oldRange = (oldMax_ - oldMin_);
			float newRange = (newMax_ - newMin_);
			return (((value_ - oldMin_) * newRange) / oldRange) + newMin_;
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