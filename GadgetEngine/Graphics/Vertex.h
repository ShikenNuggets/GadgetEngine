#ifndef GADGET_VERTEX_H
#define GADGET_VERTEX_H

#include "Math/Vector.h"

namespace Gadget{
	class Vertex{
	public:
		constexpr Vertex(const Vector3& pos_, const Vector3& norm_, const Vector2& tex_) : position(pos_), normal(norm_), texCoords(tex_){}

		Vector3 position;
		Vector3 normal;
		Vector2 texCoords;
	};
}

#endif //!GADGET_VERTEX_H