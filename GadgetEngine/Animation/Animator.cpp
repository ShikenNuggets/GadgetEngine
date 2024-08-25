#include "Animator.h"

#include "App.h"

using namespace Gadget;

Animator::Animator(StringID animMeshName_, const Skeleton& skeleton_, const Array<StringID>& clipNames_) : animMeshName(animMeshName_), skeleton(skeleton_), globalTime(0.0f), skeletonInstance(), clips(), currentClip(nullptr), currentPosNodes(), currentRotNodes(), currentScaleNodes(), globalTransformCache(){
	GADGET_BASIC_ASSERT(skeleton.GetGlobalInverse().IsValid());
	GADGET_BASIC_ASSERT(skeleton.IsValidSkeleton());
	GADGET_BASIC_ASSERT(!clipNames_.IsEmpty());

	AnimMesh* animMeshPtr = App::GetResourceManager().LoadResource<AnimMesh>(animMeshName_); //Claim ownership of the mesh so we can keep the skeleton loaded
	GADGET_BASIC_ASSERT(animMeshPtr != nullptr);
	
	for(int32_t i = 0; i < skeleton.GetJointCount(); i++){
		skeletonInstance.Add(Matrix4::Identity());

		currentPosNodes.Add(skeleton.GetJoint(i).name, nullptr);
		currentRotNodes.Add(skeleton.GetJoint(i).name, nullptr);
		currentScaleNodes.Add(skeleton.GetJoint(i).name, nullptr);
	}

	for(const auto& name : clipNames_){
		clips.Add(name, App::GetResourceManager().LoadResource<AnimClip>(name));
		GADGET_BASIC_ASSERT(clips[name] != nullptr);
	}

	globalTransformCache.Reserve(skeleton.GetJointCount());
}

Animator::~Animator(){
	App::GetResourceManager().UnloadResource(animMeshName);

	const Array<StringID> clipNames = clips.Keys();
	for(const auto& c : clipNames){
		App::GetResourceManager().UnloadResource(c);
	}
}

void Animator::Update(float deltaTime_){
	GADGET_BASIC_ASSERT(Math::IsValidNumber(deltaTime_));

	globalTime += deltaTime_;

	if(currentClip == nullptr){
		return;
	}

	UpdateSkeletonInstance(currentClip, globalTime);
}

Matrix4 Animator::GetJointTransform(int32_t jointID_) const{
	GADGET_BASIC_ASSERT(jointID_ >= 0);
	GADGET_BASIC_ASSERT(jointID_ < skeletonInstance.Size());
	if(jointID_ < 0 || jointID_ >= skeletonInstance.Size()){
		GADGET_LOG_WARNING(SID("ANIM"), "Tried to get invalid joint transform at index " + std::to_string(jointID_));
		return Matrix4::Identity();
	}

	return skeletonInstance[jointID_];
}

void Animator::UpdateSkeletonInstance(AnimClip* clip_, float time_){
	GADGET_BASIC_ASSERT(clip_ != nullptr);
	GADGET_BASIC_ASSERT(Math::IsValidNumber(time_));
	if(clip_ == nullptr){
		return;
	}

	globalTransformCache.Clear();

	for(int32_t i = 0; i < skeletonInstance.Size(); i++){
		const Joint& joint = skeleton.GetJoint(i);
		GADGET_BASIC_ASSERT(joint.name != StringID::None);
		GADGET_BASIC_ASSERT(joint.parentID >= -1);
		GADGET_BASIC_ASSERT(joint.inverseBindPose.IsValid());

		auto result = clip_->GetTransformAtTime(joint.name, time_, currentPosNodes[joint.name], currentRotNodes[joint.name], currentScaleNodes[joint.name]);
		currentPosNodes[joint.name] = result.posNode;
		currentRotNodes[joint.name] = result.rotNode;
		currentScaleNodes[joint.name] = result.scaleNode;

		int32_t parentID = joint.parentID;
		Matrix4 parentTransform = Matrix4::Identity();
		if(joint.parentID >= 0){
			parentTransform = globalTransformCache[parentID];
		}

		globalTransformCache.Add(parentTransform * result.result);
		skeletonInstance[i] = skeleton.GetGlobalInverse() * globalTransformCache[i] * joint.inverseBindPose;
	}
}