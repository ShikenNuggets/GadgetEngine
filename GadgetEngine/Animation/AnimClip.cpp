#include "AnimClip.h"

using namespace Gadget;

bool AnimClip::HasKeysForJoint(StringID jointName_) const{ return posKeys.Contains(jointName_) || rotKeys.Contains(jointName_) || scaleKeys.Contains(jointName_); }

FullClipSearchResult AnimClip::GetTransformAtTime(StringID name_, float time_, const VectorNode* posNode_, const QuatNode* rotNode_, const VectorNode* scaleNode_) const{
	return FullClipSearchResult(
		GetTranslationAtTime(name_, time_, posNode_),
		GetRotationAtTime(name_, time_, rotNode_),
		GetScaleAtTime(name_, time_, scaleNode_)
	);
}

VectorResult AnimClip::GetTranslationAtTime(StringID name_, float time_, const VectorNode* posNode_) const{
	if(!posKeys.Contains(name_)){
		return VectorResult(nullptr);
	}

	const auto& keys = posKeys[name_];
	if(keys.IsEmpty()){
		return VectorResult(nullptr);
	}

	if(keys.Size() == 1 || Math::IsNearZero(time_)){
		return VectorResult(keys.Front());
	}

	if(time_ >= keys.Back()->value.time){
		return VectorResult(keys.Back());
	}

	const auto* node = posNode_;
	if(node == nullptr){
		node = keys.Front();
	}
	
	return VectorResult(Search<VectorKey>(node, time_));
}

QuatResult AnimClip::GetRotationAtTime(StringID name_, float time_, const QuatNode* rotNode_) const{
	if(!rotKeys.Contains(name_)){
		return QuatResult(nullptr);
	}

	const auto& keys = rotKeys[name_];
	if(keys.IsEmpty()){
		return QuatResult(nullptr);
	}

	if(keys.Size() == 1 || Math::IsNearZero(time_)){
		return QuatResult(keys.Front());
	}

	if(time_ >= keys.Back()->value.time){
		return QuatResult(keys.Back());
	}

	const auto* node = rotNode_;
	if(node == nullptr){
		node = keys.Front();
	}

	return QuatResult(Search<QuatKey>(node, time_));
}

VectorResult AnimClip::GetScaleAtTime(StringID name_, float time_, const VectorNode* scaleNode_) const{
	if(!scaleKeys.Contains(name_)){
		return VectorResult(nullptr);
	}

	const auto& keys = scaleKeys[name_];
	if(keys.IsEmpty()){
		return VectorResult(nullptr);
	}

	if(keys.Size() == 1 || Math::IsNearZero(time_)){
		return VectorResult(keys.Front());
	}

	if(time_ >= keys.Back()->value.time){
		return VectorResult(keys.Back());
	}

	const auto* node = scaleNode_;
	if(node == nullptr){
		node = keys.Front();
	}

	return VectorResult(Search<VectorKey>(node, time_));
}