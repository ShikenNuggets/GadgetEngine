#ifndef GADGET_AUDIO_FMOD_AUDIO_CLIP_LOADER_H
#define GADGET_AUDIO_FMOD_AUDIO_CLIP_LOADER_H

#include "Utils/Utils.h"

namespace Gadget{
	class AudioClip;

	class FMODAudioClipLoader{
	public:
		STATIC_CLASS(FMODAudioClipLoader);

		static AudioClip* LoadClip(const std::string& str_);
	};
}

#endif //!GADGET_AUDIO_FMOD_AUDIO_CLIP_LOADER_H