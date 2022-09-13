#ifndef GADGET_LIGHT_SOURCE_H
#define GADGET_LIGHT_SOURCE_H

#include "Color.h"
#include "Math/Vector.h"

namespace Gadget{
	class LightSource{
	public:
		LightSource(const Vector3& pos_ = Vector3::Zero(), const Color& color_ = Color::White()) : position(pos_), color(color_){}

		Vector3 GetPosition() const{ return position; }
		Color GetColor() const{ return color; }

	protected:
		Vector3 position; //TODO - We may want to grab the position from somewhere else in the future
		Color color;
	};
}

#endif //!GADGET_LIGHT_SOURCE_H9