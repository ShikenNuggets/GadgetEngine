#ifndef GADGET_ANIMATION_ANIM_VERTEX_H
#define GADGET_ANIMATION_ANIM_VERTEX_H

#include "Data/Array.h"
#include "Graphics/Vertex.h"
#include "Math/Vector.h"

namespace Gadget{
	struct AnimVertex{
		AnimVertex(const Vector3& pos_, const Vector3& normal_, const Vector2& texCoords_) : vertex(pos_, normal_, texCoords_){
			jointIDs.Reserve(maxJointWeights);
			jointWeights.Reserve(maxJointWeights);
		}

		static constexpr int maxJointWeights = 4;

		Vertex vertex;
		Array<int32_t> jointIDs; //TODO - These should be StaticArrays
		Array<float> jointWeights;
	};
}

#endif //!GADGET_ANIMATION_ANIM_VERTEX_H