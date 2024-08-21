#ifndef GADGET_ANIMATION_ASSIMP_ANIM_LOADER_H
#define GADGET_ANIMATION_ASSIMP_ANIM_LOADER_H

#include <string>

//Hide warnings from external code that we can't/won't modify
#pragma warning(disable : 4244)		//Possible loss of data from type conversion
#pragma warning(disable : 26451)	//Possible arithmetic overflow
#include <assimp/scene.h>
#pragma warning(default : 4244)
#pragma warning(default : 26451)

#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace Gadget{
	class AnimClip;

	class AssimpAnimLoader{
	public:
		static unsigned int GetNumAnimClipsInFile(const std::string& filePath_);
		static AnimClip* LoadAnimClip(const std::string& filePath_, unsigned int clipID_);

	private:
		static inline Vector3 ConvertVector3(const aiVector3D& vec_){ return Vector3(vec_.x, vec_.y, vec_.z); }
		static inline Quaternion ConvertQuaternion(const aiQuaternion& quat_){ return Quaternion(quat_.w, quat_.x, quat_.y, quat_.z); }
	};
}

#endif //!GADGET_ANIMATION_ASSIMP_ANIM_LOADER_H