#ifndef GADGET_ANIMATION_ANIMATOR_H
#define GADGET_ANIMATION_ANIMATOR_H

#include <unordered_map>

#include "Animation/AnimClip.h"
#include "Animation/AnimMesh.h"
#include "Animation/Skeleton.h"

namespace Gadget{
	class Animator{
	public:
		Animator(StringID animMeshName_, const Array<StringID>& clipNames_);
		~Animator();

		void Update(float deltaTime_);

		void AddClip(StringID clipName_);
		void PlayClip(StringID clipName_);
		void Stop();

		const Skeleton& GetSkeleton() const{
			GADGET_BASIC_ASSERT(skeleton != nullptr);
			return *skeleton;
		}

		inline const Array<Matrix4>& GetCurrentSkeletonInstance() const{ return skeletonInstance; }

		Matrix4 GetJointTransform(int32_t jointID_) const;

	private:
		StringID animMeshName;
		const Skeleton* skeleton;
		float globalTime;
		Array<Matrix4> skeletonInstance;

		std::unordered_map<StringID, AnimClip*> clips;
		AnimClip* currentClip;

		std::unordered_map<StringID, const VectorNode*> currentPosNodes;
		std::unordered_map<StringID, const QuatNode*> currentRotNodes;
		std::unordered_map<StringID, const VectorNode*> currentScaleNodes;

		Array<Matrix4> globalTransformCache;

		virtual void UpdateSkeletonInstance(AnimClip* clip_, float time_);

		void ClearCurrentNodes();
	};
}

#endif //!GADGET_ANIMATION_ANIMATOR_H