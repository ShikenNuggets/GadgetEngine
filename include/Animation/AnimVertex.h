#ifndef GADGET_ANIMATION_ANIM_VERTEX_H
#define GADGET_ANIMATION_ANIM_VERTEX_H

#include "Data/StaticArray.h"
#include "Graphics/Vertex.h"
#include "Math/Vector.h"

namespace Gadget{
	struct AnimVertex{
		static constexpr int maxJointWeights = 4;

		Vertex vertex;
		StaticArray<int32_t, maxJointWeights> jointIDs;
		StaticArray<float, maxJointWeights> jointWeights;

		AnimVertex(const Vector3& pos_, const Vector3& normal_, const Vector2& texCoords_) : vertex(pos_, normal_, texCoords_), jointIDs{ -1, -1, -1, -1 }, jointWeights{0.0f, 0.0f, 0.0f, 0.0f}{}

		void AddWeight(int32_t jointID_, float weight_){
			for(int i = 0; i < maxJointWeights; i++){
				if(jointIDs[i] < 0){
					jointIDs[i] = jointID_;
					jointWeights[i] = weight_;
					return;
				}
			}

			GADGET_LOG_ERROR(SID("ANIM"), "Tried to add more than " + Utils::Stringify(maxJointWeights) + " joint weights to a vertex!");
		}
	};
}

#endif //!GADGET_ANIMATION_ANIM_VERTEX_H