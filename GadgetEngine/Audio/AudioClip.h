#ifndef GADGET_AUDIO_AUDIO_CLIP_H
#define GADGET_AUDIO_AUDIO_CLIP_H

#include <fmod.hpp>

#include "Debug.h"
#include "Audio/FMODAudioClipLoader.h"
#include "Core/FileSystem.h"
#include "Resource/Resource.h"

namespace Gadget{
	class AudioClip : public Resource{
	public:
		AudioClip(FMOD::Sound* sound2D_, FMOD::Sound* sound3D_) : sound2D(sound2D_), sound3D(sound3D_){
			GADGET_BASIC_ASSERT(sound2D != nullptr);
			GADGET_BASIC_ASSERT(sound3D != nullptr);
		}

		static constexpr const char* typeName = "AudioClip";

		FMOD::Sound* GetSound2D(){ return sound2D; }
		FMOD::Sound* GetSound3D(){ return sound3D; }

	private:
		FMOD::Sound* sound2D;
		FMOD::Sound* sound3D;
	};

	class AudioClipResourceContainer : public ResourceContainer{
	public:
		AudioClipResourceContainer(const std::string& path_) : ResourceContainer(AudioClip::typeName, path_){
			GADGET_BASIC_ASSERT(!path_.empty());
			GADGET_BASIC_ASSERT(FileSystem::FileExists(path_));
		}

		std::string GetPath() const{ return path; }

		virtual Resource* LoadResource() override{
			return FMODAudioClipLoader::LoadClip(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(AudioClip);
		}
	};
}

#endif //!GADGET_AUDIO_AUDIO_CLIP_H