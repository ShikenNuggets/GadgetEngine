#ifndef GADGET_ANIMATION_JOINT_H
#define GADGET_ANIMATION_JOINT_H

#include <cstdint>

#include "Data/Array.h"
#include "Math/Matrix.h"

namespace Gadget{
	struct Joint{
		explicit Joint() : name(StringID::None), parentID(-1), inverseBindPose(Matrix4::Identity()){}

		StringID name;
		int32_t parentID;
		Matrix4 inverseBindPose;
	};

	class Skeleton{
	public:
		Skeleton() = default;

		void AddJoint(const Joint& joint_);
		void AddJoint(StringID name_, int32_t parentID_, const Matrix4& inverseBindPose_);
		
		size_t GetJointCount() const{ return joints.Size(); }
		int32_t GetJointID(StringID name_) const;

		const Joint& GetJoint(StringID name_) const;
		const Joint& GetJoint(int32_t id_) const;

		bool HasJoint(StringID name_) const;

		bool IsValidSkeleton() const;

	private:
		Array<Joint> joints;
	};
}

#endif //!GADGET_ANIMATION_JOINT_H