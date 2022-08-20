#ifndef GADGET_COLOR_H
#define GADGET_COLOR_H

namespace Gadget{
	struct Color{
		constexpr Color(float r_, float g_, float b_, float a_ = 1.0f) : r(r_), g(g_), b(b_), a(a_){}

		float r, g, b, a;

		static inline constexpr Color Red()		{ return Color(1.0f, 0.0f, 0.0f, 1.0f); }
		static inline constexpr Color Green()	{ return Color(0.0f, 1.0f, 0.0f, 1.0f); }
		static inline constexpr Color Blue()	{ return Color(0.0f, 0.0f, 1.0f, 1.0f); }
		static inline constexpr Color Yellow ()	{ return Color(1.0f, 1.0f, 0.0f, 1.0f); }
		static inline constexpr Color Orange()	{ return Color(1.0f, 0.5f, 0.0f, 1.0f); }
		static inline constexpr Color Purple()	{ return Color(0.4f, 0.0f, 0.8f, 1.0f); }
		static inline constexpr Color Black()	{ return Color(0.0f, 0.0f, 0.0f, 1.0f); }
		static inline constexpr Color White()	{ return Color(1.0f, 1.0f, 1.0f, 1.0f); }
		static inline constexpr Color Brown()	{ return Color(0.4f, 0.2f, 0.0f, 1.0f); }
		static inline constexpr Color Gray()	{ return Color(0.5f, 0.5f, 0.5f, 1.0f); }
		static inline constexpr Color DarkGray(){ return Color(0.2f, 0.2f, 0.2f, 1.0f); }
	};
}

#endif //!GADGET_COLOR_H