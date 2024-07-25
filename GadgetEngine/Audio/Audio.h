#ifndef GADGET_AUDIO_AUDIO_H
#define GADGET_AUDIO_AUDIO_H

#include <fmod_studio.hpp>

#include "GadgetEnums.h"
#include "Game/Scene.h"

namespace Gadget{
	class Audio{
	public:
		Audio();
		~Audio();

		void Update(Scene* scene_);

		FMOD::Studio::System* GetStudioSystem(){ return studioSystem; }
		FMOD::System* GetCoreSystem(){ return coreSystem; }

		float GetVolume(VolumeChannel channel_ = VolumeChannel::Master);
		void SetVolume(float volume_, VolumeChannel channel_ = VolumeChannel::Master);

	private:
		FMOD::Studio::System* studioSystem;
		FMOD::System* coreSystem;
	};
}

#endif //!GADGET_AUDIO_AUDIO_H