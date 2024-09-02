#include "Animator.h"

#include "App.h"

using namespace Gadget;

Animator::Animator(StringID animMeshName_, const Array<StringID>& clipNames_) : animMeshName(animMeshName_), skeleton(nullptr), globalTime(0.0f), skeletonInstance(), clips(), currentClip(nullptr), currentPosNodes(), currentRotNodes(), currentScaleNodes(), globalTransformCache(){
	GADGET_BASIC_ASSERT(animMeshName != StringID::None);
	AnimMesh* animMeshPtr = App::GetResourceManager().LoadResource<AnimMesh>(animMeshName_); //Claim ownership of the mesh so we can keep the skeleton loaded
	GADGET_BASIC_ASSERT(animMeshPtr != nullptr);
	if(animMeshPtr != nullptr){
		skeleton = &animMeshPtr->skeleton;
	}

	GADGET_BASIC_ASSERT(skeleton != nullptr);
	if(skeleton == nullptr){
		Debug::ThrowFatalError(SID("ANIM"), "Skeleton must not be nullptr!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
	}

	GADGET_BASIC_ASSERT(skeleton->GetGlobalInverse().IsValid());
	GADGET_BASIC_ASSERT(skeleton->IsValidSkeleton());
	
	skeletonInstance.Reserve(skeleton->GetJointCount());
	for(int32_t i = 0; i < skeleton->GetJointCount(); i++){
		skeletonInstance.Add(Matrix4::Identity());

		currentPosNodes.Add(skeleton->GetJoint(i).name, nullptr);
		currentRotNodes.Add(skeleton->GetJoint(i).name, nullptr);
		currentScaleNodes.Add(skeleton->GetJoint(i).name, nullptr);
	}

	for(const auto& name : clipNames_){
		GADGET_BASIC_ASSERT(name != StringID::None);
		clips.Add(name, App::GetResourceManager().LoadResource<AnimClip>(name));
		GADGET_BASIC_ASSERT(clips[name] != nullptr);
	}

	globalTransformCache.Reserve(skeleton->GetJointCount());
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
	GADGET_BASIC_ASSERT(skeleton != nullptr);

	globalTime += deltaTime_;

	if(skeleton == nullptr || currentClip == nullptr){
		return;
	}

	//TODO - IsLooping
	GADGET_BASIC_ASSERT(currentClip->GetLength() > 0.0f);
	while(globalTime >= currentClip->GetLength()){
		globalTime -= currentClip->GetLength();
	}

	UpdateSkeletonInstance(currentClip, globalTime);
}

void Animator::AddClip(StringID clipName_){
	GADGET_BASIC_ASSERT(clipName_ != StringID::None);
	clips.Add(clipName_, App::GetResourceManager().LoadResource<AnimClip>(clipName_));
	GADGET_BASIC_ASSERT(clips[clipName_] != nullptr);

	if(currentClip == nullptr){
		currentClip = clips[clipName_];
	}
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
		const Joint& joint = skeleton->GetJoint(i);
		GADGET_BASIC_ASSERT(joint.name != StringID::None);
		GADGET_BASIC_ASSERT(joint.parentID >= -1);
		GADGET_BASIC_ASSERT(joint.inverseBindPose.IsValid());

		const auto result = clip_->GetTransformAtTime(joint.name, time_, currentPosNodes[joint.name], currentRotNodes[joint.name], currentScaleNodes[joint.name]);
		currentPosNodes[joint.name] = result.posNode;
		currentRotNodes[joint.name] = result.rotNode;
		currentScaleNodes[joint.name] = result.scaleNode;

		Matrix4 parentTransform = Matrix4::Identity();
		if(joint.parentID >= 0){
			GADGET_BASIC_ASSERT(joint.parentID < globalTransformCache.Size());
			parentTransform = globalTransformCache[joint.parentID];
		}

		const Matrix4 transform = parentTransform * result.result;
		globalTransformCache.Add(transform);
		skeletonInstance[i] = skeleton->GetGlobalInverse() * transform * joint.inverseBindPose;
	}
}