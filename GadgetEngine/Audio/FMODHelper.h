#ifndef GADGET_AUDIO_FMOD_HELPER_H
#define GADGET_AUDIO_FMOD_HELPER_H

#include <fmod_common.h>

#include "Math/Vector.h"

namespace FMODHelper{
	static constexpr inline Gadget::Vector3 ConvertVector3(const FMOD_VECTOR& vec_){
		return Gadget::Vector3(vec_.x, vec_.y, vec_.z);
	}

	static inline FMOD_VECTOR ConvertVector3(const Gadget::Vector3& vec_){
		GADGET_BASIC_ASSERT(vec_.IsValid());
		return FMOD_VECTOR(vec_.x, vec_.y, vec_.z);
	}
}

#endif //!GADGET_AUDIO_FMOD_HELPER_H