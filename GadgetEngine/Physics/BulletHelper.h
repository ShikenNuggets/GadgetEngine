#ifndef GADGET_BULLET_HELPER_H
#define GADGET_BULLET_HELPER_H

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 4127)		//Conditional expression is constant
#include <btBulletDynamicsCommon.h>
#pragma warning(default : 26451)
#pragma warning(default : 26495)
#pragma warning(default : 4127)

#include "Math/Math.h"
#include "Game/GameObject.h"

namespace BulletHelper{
	constexpr inline Gadget::Vector3 ConvertVector3(const btVector3& vec_){
		return Gadget::Vector3(vec_.getX(), vec_.getY(), vec_.getZ());
	}

	inline btVector3 ConvertVector3(const Gadget::Vector3& vec_){
		GADGET_BASIC_ASSERT(vec_.IsValid());
		return btVector3(vec_.x, vec_.y, vec_.z);
	}

	constexpr inline Gadget::Quaternion ConvertQuaternion(const btQuaternion& quat_){
		return Gadget::Quaternion(quat_.getW(), quat_.getX(), quat_.getY(), quat_.getZ());
	}

	inline btQuaternion ConvertQuaternion(const Gadget::Quaternion& quat_){
		GADGET_BASIC_ASSERT(quat_.IsValid());
		return btQuaternion(quat_.x, quat_.y, quat_.z, quat_.w);
	}

	inline Gadget::Transform ConvertTransform(const btTransform& transform_, const Gadget::Vector3& scale_){
		return Gadget::Transform(ConvertVector3(transform_.getOrigin()), ConvertQuaternion(transform_.getRotation()), scale_);
	}

	inline Gadget::Transform ConvertTransform(const btTransform& transform_, float scale_ = 1.0f){
		return ConvertTransform(transform_, Gadget::Vector3::Fill(scale_));
	}

	inline btTransform ConvertTransform(const Gadget::Transform& transform_){
		GADGET_BASIC_ASSERT(transform_.position.IsValid());
		GADGET_BASIC_ASSERT(transform_.rotation.IsValid());
		GADGET_BASIC_ASSERT(transform_.scale.IsValid());

		return btTransform(ConvertQuaternion(transform_.rotation), ConvertVector3(transform_.position));
	}
};

#endif //!GADGET_BULLET_HELPER_H