#include "Skeleton.h"

#include "Data/Pair.h"

using namespace Gadget;

Skeleton::Skeleton(const Matrix4& globalInverse_) : joints(), globalInverse(globalInverse_){}

void Skeleton::AddJoint(const Joint& joint_){
	GADGET_BASIC_ASSERT(!HasJoint(joint_.name));
	joints.Add(joint_);
	GADGET_ASSERT(joints.Size() < std::numeric_limits<int32_t>::max(), "Skeleton has more than 2^31 joints, you need to use a larger int type for IDs!");
}

void Skeleton::AddJoint(StringID name_, int32_t parentID_, const Matrix4& inverseBindPose_){
	GADGET_BASIC_ASSERT(!HasJoint(name_));

	Joint j;
	j.name = name_;
	j.parentID = parentID_;
	j.inverseBindPose = inverseBindPose_;
	joints.Add(j);

	GADGET_ASSERT(joints.Size() < std::numeric_limits<int32_t>::max(), "Skeleton has more than 2^31 joints, you need to use a larger int type for IDs!");
}

int32_t Skeleton::GetJointID(StringID name_) const{
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	GADGET_BASIC_ASSERT(HasJoint(name_));
	GADGET_ASSERT(joints.Size() < std::numeric_limits<int32_t>::max(), "Skeleton has more than 2^31 joints, you need to use a larger int type for IDs!");

	for(int32_t i = 0; i < joints.Size(); i++){
		if(joints[i].name == name_){
			return i;
		}
	}

	GADGET_LOG_WARNING(SID("ANIM"), "Tried to get invalid joint [" + name_.GetString() + "]!");
	return -1;
}

const Joint& Skeleton::GetJoint(StringID name_) const{
	GADGET_BASIC_ASSERT(name_ != StringID::None);
	GADGET_BASIC_ASSERT(HasJoint(name_));

	for(const auto& j : joints){
		if(j.name == name_){
			return j;
		}
	}

	//There's no safe way to handle this, so just error out
	Debug::ThrowFatalError(SID("ANIM"), "Tried to get invalid joint [" + name_.GetString() + "]!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
}

const Joint& Skeleton::GetJoint(int32_t id_) const{
	GADGET_BASIC_ASSERT(id_ >= 0);
	GADGET_BASIC_ASSERT(id_ < joints.Size());

	if(id_ < 0 || id_ >= joints.Size()){
		//There's no safe way to handle this, so just error out
		Debug::ThrowFatalError(SID("ANIM"), "Tried to get invalid joint at index " + std::to_string(id_) + "!", ErrorCode::Invalid_Args, __FILE__, __LINE__);
	}

	return joints[id_];
}

Matrix4 Skeleton::GetGlobalInverse() const{ return globalInverse; }

bool Skeleton::HasJoint(StringID name_) const{
	GADGET_BASIC_ASSERT(name_ != StringID::None);

	for(const auto& j : joints){
		if(j.name == name_){
			return true;
		}
	}

	return false;
}

bool Skeleton::IsValidSkeleton() const{
	GADGET_ASSERT(joints.Size() < std::numeric_limits<int32_t>::max(), "Skeleton has more than 2^31 joints, you need to use a larger int type for IDs!");
	
	bool foundRootNode = false;
	Array<Pair<int32_t, bool>> confirmedJoints;
	for(int32_t i = 0; i < joints.Size(); i++){
		confirmedJoints.Add(Pair(i, false));
	}

	for(int32_t i = 0; i < joints.Size(); i++){
		const Joint& currentJoint = joints[i];

		if(currentJoint.parentID == i){
			GADGET_LOG_WARNING(SID("ANIM"), "Skeleton joint [" + currentJoint.name.GetString() + "] is parented to itself");
			return false;
		}

		if(currentJoint.parentID < -1){
			GADGET_LOG_WARNING(SID("ANIM"), "Skeleton joint [" + currentJoint.name.GetString() + "] has an invalid parent ID");
			return false; //-1 denotes no parent, anything lower probably means something went wrong
		}

		if(currentJoint.parentID == -1){
			if(foundRootNode){
				GADGET_LOG_WARNING(SID("ANIM"), "Skeleton joint [" + currentJoint.name.GetString() + "] identified as a root node, but skeleton already has a root node!");
				return false; //Skeleton has multiple root nodes
			}else{
				foundRootNode = true;
			}
		}

		if(currentJoint.parentID >= 0 && confirmedJoints[currentJoint.parentID].second == false){
			GADGET_LOG_WARNING(SID("ANIM"), "Skeleton joint ]" + currentJoint.name.GetString() + "]'s parent was not found");
			return false; //Parent was not found yet
		}

		confirmedJoints[i].second = true;
	}

	return true;
}