#ifndef GADGET_AUDIO_AUDIO_H
#define GADGET_AUDIO_AUDIO_H

#include <vector>

#include <fmod_studio.hpp>

#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
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

		//Member variables so we can reuse their memory, but DO NOT reuse their contents!
		std::vector<AudioListener*> audioListenersBuffer;
		std::vector<AudioSource*> audioSourcesBuffer;
	};
}

#endif //!GADGET_AUDIO_AUDIO_H