#include <btBulletDynamicsCommon.h>

#include "Game/GameObject.h"

namespace BulletHelper{
	constexpr inline Gadget::Vector3 ConvertVector3(const btVector3& vec_){
		return Gadget::Vector3(vec_.getX(), vec_.getY(), vec_.getZ());
	}

	inline btVector3 ConvertVector3(const Gadget::Vector3& vec_){
		return btVector3(vec_.x, vec_.y, vec_.z);
	}

	constexpr inline Gadget::Quaternion ConvertQuaternion(const btQuaternion& quat_){
		return Gadget::Quaternion(quat_.getW(), quat_.getX(), quat_.getY(), quat_.getZ());
	}

	inline btQuaternion ConvertQuaternion(const Gadget::Quaternion& quat_){
		return btQuaternion(quat_.x, quat_.y, quat_.z, quat_.w);
	}

	constexpr inline Gadget::Transform ConvertTransform(const btTransform& transform_, const Gadget::Vector3& scale_){
		return Gadget::Transform(ConvertVector3(transform_.getOrigin()), ConvertQuaternion(transform_.getRotation()));
	}

	constexpr inline Gadget::Transform ConvertTransform(const btTransform& transform_, float scale_ = 1.0f){
		return ConvertTransform(transform_, Gadget::Vector3::Fill(scale_));
	}

	inline btTransform ConvertTransform(const Gadget::Transform& transform_){
		return btTransform(ConvertQuaternion(transform_.rotation), ConvertVector3(transform_.position));
	}
};