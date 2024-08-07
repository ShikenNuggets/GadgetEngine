#ifndef GADGET_COLOR_H
#define GADGET_COLOR_H

#include "Math/Math.h"

namespace Gadget{
	struct Color{
		constexpr Color(float r_, float g_, float b_, float a_ = 1.0f) noexcept : r(r_), g(g_), b(b_), a(a_){
			r = Math::Clamp(0.0f, 1.0f, r_);
			g = Math::Clamp(0.0f, 1.0f, g_);
			b = Math::Clamp(0.0f, 1.0f, b_);
			a = Math::Clamp(0.0f, 1.0f, a_);
		}

		float r, g, b, a;

		static inline constexpr Color Red()		{ return Color(1.0f, 0.0f, 0.0f, 1.0f); }
		static inline constexpr Color Green()	{ return Color(0.0f, 1.0f, 0.0f, 1.0f); }
		static inline constexpr Color Blue()	{ return Color(0.0f, 0.0f, 1.0f, 1.0f); }
		static inline constexpr Color Yellow ()	{ return Color(1.0f, 1.0f, 0.0f, 1.0f); }
		static inline constexpr Color Orange()	{ return Color(1.0f, 0.5f, 0.0f, 1.0f); }
		static inline constexpr Color Purple()	{ return Color(0.4f, 0.0f, 0.8f, 1.0f); }
		static inline constexpr Color Pink()	{ return Color(1.0f, 0.0f, 1.0f, 1.0f); }
		static inline constexpr Color Black()	{ return Color(0.0f, 0.0f, 0.0f, 1.0f); }
		static inline constexpr Color White()	{ return Color(1.0f, 1.0f, 1.0f, 1.0f); }
		static inline constexpr Color Brown()	{ return Color(0.4f, 0.2f, 0.0f, 1.0f); }
		static inline constexpr Color Gray()	{ return Color(0.5f, 0.5f, 0.5f, 1.0f); }
		static inline constexpr Color DarkGray(){ return Color(0.2f, 0.2f, 0.2f, 1.0f); }

		//These allow us to pass this as an array to legacy code or things like OpenGL very easily
		inline constexpr operator const float* () const{ return static_cast<const float*>(&r); }
		inline constexpr operator float* (){ return static_cast<float*>(&r); }

		inline bool IsValid() const{ return Math::IsValidNumber(r) && Math::IsValidNumber(g) && Math::IsValidNumber(b) && Math::IsValidNumber(a); }
	};
}

#endif //!GADGET_COLOR_H