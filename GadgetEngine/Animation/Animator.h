#ifndef GADGET_ANIMATION_ANIMATOR_H
#define GADGET_ANIMATION_ANIMATOR_H

#include "Animation/AnimClip.h"
#include "Animation/AnimMesh.h"
#include "Animation/Skeleton.h"
#include "Data/HashTable.h"

namespace Gadget{
	class Animator{
	public:
		Animator(StringID animMeshName_, const Array<StringID>& clipNames_);
		~Animator();

		void Update(float deltaTime_);

		void AddClip(StringID clipName_);

		const Skeleton& GetSkeleton() const{
			GADGET_BASIC_ASSERT(skeleton != nullptr);
			return *skeleton;
		}

		inline const Array<Matrix4>& GetCurrentSkeletonInstance() const{ return skeletonInstance; }

		Matrix4 GetJointTransform(int32_t jointIndex_) const;

	private:
		StringID animMeshName;
		const Skeleton* skeleton;
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