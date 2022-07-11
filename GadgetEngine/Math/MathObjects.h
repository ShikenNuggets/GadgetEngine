#ifndef GADGET_MATH_OBJECTS_H
#define GADGET_MATH_OBJECTS_H

#include "Vector.h"

namespace Gadget{
	struct Line{
		Vector3 point;
		Vector3 dir;

		constexpr Line(const Vector3& point_, const Vector3& dir_) : point(point_), dir(dir_){}
	};

	struct Sphere{
		Vector3 point;
		float radius;

		constexpr Sphere(const Vector3& point_, float radius_) : point(point_), radius(radius_){}
	};

	struct Plane{
		Vector3 point;
		Vector3 normal;

		constexpr Plane(const Vector3& point_, const Vector3& normal_) : point(point_), normal(normal_){}
	};

	struct Rect{
		float x;
		float y;
		float w;
		float h;
		
		constexpr Rect(float x_ = 0.0f, float y_ = 0.0f, float w_ = 0.0f, float h_ = 0.0f) : x(x_), y(y_), w(w_), h(h_){};

		inline constexpr bool Intersects(float x_, float y_){
			const float halfWidth = w / 2.0f;
			const float halfHeight = h / 2.0f;
			return (x_ > x - halfWidth && x_ < x + halfWidth) && (y_ < (1.0f - y) + halfHeight && y_ >(1.0f - y) - halfHeight);
		}

		inline constexpr bool Intersects(const Vector2& v_){
			return Intersects(v_.x, v_.y);
		}
	};

	struct Cube{
		Vector3 min;
		Vector3 max;

		constexpr Cube(const Vector3& min_, const Vector3& max_) : min(min_), max(max_){}
		constexpr Cube(float minX_, float minY_, float minZ_, float maxX_, float maxY_, float maxZ_) : min(minX_, minY_, minZ_), max(maxX_, maxY_, maxZ_){}

		inline constexpr bool Intersects(float x_, float y_, float z_){
			return (x_ >= min.x && x_ <= max.x) && (y_ >= min.y && y_ <= max.y) && (z_ >= min.z && z_ <= max.z);
		}

		inline constexpr bool Intersects(const Vector3& v_){
			return Intersects(v_.x, v_.y, v_.z);
		}
	};
}

#endif //!GADGET_MATH_OBJECTS_H