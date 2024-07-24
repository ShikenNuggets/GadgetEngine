#include "FMODAudioClipLoader.h"

#include <fmod_errors.h>

#include "App.h"
#include "Audio/AudioClip.h"

using namespace Gadget;

AudioClip* FMODAudioClipLoader::LoadClip(const std::string& str_){
	FMOD::System* coreSystem = App::GetAudio().GetCoreSystem();
	GADGET_BASIC_ASSERT(coreSystem != nullptr);

	FMOD::Sound* sound2D = nullptr;
	FMOD::Sound* sound3D = nullptr;

	//Load the sound as a 2D sound
	FMOD_RESULT result = coreSystem->createSound(str_.c_str(), FMOD_2D, 0, &sound2D);
	if(result != FMOD_OK || sound2D == nullptr){
		Debug::Log(SID("AUDIO"), "2D Sound could not be created from " + str_ + "! FMOD Error Code: " + FMOD_ErrorString(result), Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	//Load the sound as a 3D sound
	result = coreSystem->createSound(str_.c_str(), FMOD_3D, 0, &sound3D);
	if(result != FMOD_OK || sound3D == nullptr){
		Debug::Log(SID("AUDIO"), "3D Sound could not be created from " + str_ + "! FMOD Error Code: " + FMOD_ErrorString(result), Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	return new AudioClip(sound2D, sound3D);
}