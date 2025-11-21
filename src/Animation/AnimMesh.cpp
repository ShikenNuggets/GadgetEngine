#include "AnimMesh.h"

using namespace Gadget;

static constexpr double gWeightErrorTolerance = 0.0000001;

AnimMesh::AnimMesh(const Array<AnimSubmesh>& submeshes_, const Skeleton& skeleton_) : submeshes(submeshes_), skeleton(skeleton_){
#ifdef GADGET_DEBUG //Would be nice to do data validation in Release mode, but these are non-trivial
	if(!skeleton.IsValidSkeleton()){
		GADGET_LOG_WARNING(SID("ANIM"), "Invalid data found in skeleton!");
	}

	for(const auto& mesh : submeshes){
		for(const auto& v : mesh.vertices){
			GADGET_BASIC_ASSERT(v.jointIDs[0] >= -1 && v.jointIDs[0] < skeleton_.GetJointCount());
			GADGET_BASIC_ASSERT(v.jointWeights[0] >= 0.0f && v.jointWeights[0] <= 1.0f);
			GADGET_BASIC_ASSERT(v.jointIDs[1] >= -1 && v.jointIDs[1] < skeleton_.GetJointCount());
			GADGET_BASIC_ASSERT(v.jointWeights[1] >= 0.0f && v.jointWeights[1] <= 1.0f);
			GADGET_BASIC_ASSERT(v.jointIDs[2] >= -1 && v.jointIDs[2] < skeleton_.GetJointCount());
			GADGET_BASIC_ASSERT(v.jointWeights[2] >= 0.0f && v.jointWeights[2] <= 1.0f);
			GADGET_BASIC_ASSERT(v.jointIDs[3] >= -1 && v.jointIDs[3] < skeleton_.GetJointCount());
			GADGET_BASIC_ASSERT(v.jointWeights[3] >= 0.0f && v.jointWeights[3] <= 1.0f);

			if(v.jointIDs[0] == -1){
				if(v.jointIDs[1] >= 0 || v.jointIDs[2] >= 0 || v.jointIDs[3] >= 0){
					GADGET_LOG_WARNING(SID("ANIM"), "AnimMesh vertex joint IDs are invalid!");
				}else{
					GADGET_LOG_WARNING(SID("ANIM"), "AnimMesh vertex has no joint weights!");
				}

				break;
			}

			double totalJointWeight = v.jointWeights[0];

			if(v.jointIDs[1] >= 0){
				totalJointWeight += v.jointWeights[1];
			}else if(v.jointIDs[2] >= 0 || v.jointIDs[3] >= 0){
				GADGET_LOG_WARNING(SID("ANIM"), "AnimMesh vertex joint IDs are invalid!");
			}

			if(v.jointIDs[2] >= 0){
				totalJointWeight += v.jointWeights[2];
			}else if(v.jointIDs[3] >= 0){
				GADGET_LOG_WARNING(SID("ANIM"), "AnimMesh vertex joint IDs are invalid!");
			}

			if(v.jointIDs[3] >= 0){
				totalJointWeight += v.jointWeights[3];
			}

			if(totalJointWeight < 1.0 - gWeightErrorTolerance || totalJointWeight > 1.0 + gWeightErrorTolerance){
				GADGET_LOG_WARNING(SID("ANIM"), "Joint weights for AnimVertex added up to " + std::to_string(totalJointWeight) + ", expected 1.0");
			}
		}
	}
#endif //GADGET_DEBUG
}