#ifndef GADGET_SCREEN_COORDINATE_H
#define GADGET_SCREEN_COORDINATE_H

namespace Gadget{
	struct ScreenCoordinate{
		int x;
		int y;

		explicit constexpr ScreenCoordinate(int x_, int y_) noexcept : x(x_), y(y_){}

		inline constexpr bool operator ==(const ScreenCoordinate& sc_) const{ return x == sc_.x && y == sc_.y; }
		inline constexpr bool operator !=(const ScreenCoordinate& sc_) const{ return x != sc_.x && y != sc_.y; }

		inline constexpr ScreenCoordinate operator +(const ScreenCoordinate& sc_) const{ return ScreenCoordinate(x + sc_.x, y + sc_.y); }
		inline constexpr ScreenCoordinate operator -(const ScreenCoordinate& sc_) const{ return ScreenCoordinate(x - sc_.x, y - sc_.y); }

		inline constexpr void operator +=(const ScreenCoordinate& sc_){ *this = *this + sc_; }
		inline constexpr void operator -=(const ScreenCoordinate& sc_){ *this = *this - sc_; }
	};
}

#endif //!GADGET_SCREEN_COORDINATE_H