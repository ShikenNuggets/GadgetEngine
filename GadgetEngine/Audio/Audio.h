#ifndef GADGET_AUDIO_AUDIO_H
#define GADGET_AUDIO_AUDIO_H

#include <fmod_studio.hpp>

namespace Gadget{
	class Audio{
	public:
		Audio();
		~Audio();

		void Update();

	private:
		FMOD::Studio::System* studioSystem;
		FMOD::System* coreSystem;
	};
}

#endif //!GADGET_AUDIO_AUDIO_H