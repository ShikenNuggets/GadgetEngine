#ifndef GADGET_ANIMATION_ANIMATOR_H
#define GADGET_ANIMATION_ANIMATOR_H

#include "Animation/AnimClip.h"
#include "Animation/AnimMesh.h"
#include "Animation/Skeleton.h"
#include "Data/HashTable.h"

namespace Gadget{
	class Animator{
	public:
		Animator(const AnimMesh& mesh_, const Array<StringID>& clipNames_);
		~Animator();

		void Update(float deltaTime_);

		const Skeleton& GetSkeleton() const{ return animMesh.skeleton; }

		Matrix4 GetJointTransform(int32_t jointIndex_) const;

	private:
		const AnimMesh& animMesh;
		float globalTime;
		Array<Matrix4> skeletonInstance;

		HashTable<StringID, AnimClip*> clips;
		AnimClip* currentClip;

		HashTable<StringID, const VectorNode*> currentPosNodes;
		HashTable<StringID, const QuatNode*> currentRotNodes;
		HashTable<StringID, const VectorNode*> currentScaleNodes;

		Array<Matrix4> globalTransformCache;

		virtual void UpdateSkeletonInstance(AnimClip* clip_, float time_);
	};
}

#endif //!GADGET_ANIMATION_ANIMATOR_H