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

//I apologize in advance to anyone hoping to understand or debug this code
template <class T>
inline const DList<T>::Node* Search(const typename DList<T>::Node* startNode_, float time_){
	const auto* curNode = startNode_;
	GADGET_BASIC_ASSERT(curNode != nullptr);
	if(curNode == nullptr){
		return nullptr;
	}

	if(time_ == curNode->value.time){
		//Current node is already at the time we're looking for
		return curNode;
	}else{
		const bool searchForward = time_ >= curNode->value.time;
		while(curNode != nullptr){
			if(curNode->value.time <= time_ && (curNode->next == nullptr || curNode->next->value.time >= time_)){
				return curNode;
			}

			if(searchForward){
				curNode = curNode->next;
			}else{
				curNode = curNode->prev;
			}
		}
	}

	GADGET_ASSERT_UNREACHABLE;
	return nullptr;
}

template <class ResultT, class KeyT, class NodeT, class KeysT>
inline ResultT GetResultAtTime(StringID name_, float time_, const NodeT* node_, const KeysT& keys_){
	if(!keys_.Contains(name_)){
		//Default initialize the result if there are no keys for this joint
		return ResultT(nullptr);
	}

	const auto& keys = keys_[name_];
	if(keys.IsEmpty()){
		//Default initialize the result if there are no keys for this joint
		GADGET_LOG_WARNING(SID("ANIM"), "List created for joint " + name_.GetString() + " with no keyframes!");
		return ResultT(nullptr);
	}

	if(keys.Size() == 1 || Math::IsNearZero(time_)){
		return ResultT(keys.Front(), time_);
	}

	if(time_ >= keys.Back()->value.time){
		return ResultT(keys.Back(), time_);
	}

	//If we don't have a cached node, start at the front
	if(node_ == nullptr){
		node_ = keys.Front();
	}

	return ResultT(Search<KeyT>(node_, time_), time_);
}

VectorResult AnimClip::GetTranslationAtTime(StringID name_, float time_, const VectorNode* posNode_) const{
	return GetResultAtTime<VectorResult, VectorKey>(name_, time_, posNode_, posKeys);
}

QuatResult AnimClip::GetRotationAtTime(StringID name_, float time_, const QuatNode* rotNode_) const{
	return GetResultAtTime<QuatResult, QuatKey>(name_, time_, rotNode_, rotKeys);
}

VectorResult AnimClip::GetScaleAtTime(StringID name_, float time_, const VectorNode* scaleNode_) const{
	return GetResultAtTime<VectorResult, VectorKey>(name_, time_, scaleNode_, scaleKeys);
}