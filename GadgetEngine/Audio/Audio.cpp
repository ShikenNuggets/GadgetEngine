#include "Audio.h"

#include <fmod_errors.h>

#include "Debug.h"
#include "GadgetEnums.h"

using namespace Gadget;

Audio::Audio() : studioSystem(nullptr), coreSystem(nullptr){
	FMOD_RESULT result = FMOD::Studio::System::create(&studioSystem);
	if(result != FMOD_OK || studioSystem == nullptr){
		Debug::ThrowFatalError(SID("AUDIO"), "FMOD Studio System could not be created! FMOD Error: " + std::string(FMOD_ErrorString(result)), ErrorCode::FMOD_Init_Error, __FILE__, __LINE__);
	}

	result = studioSystem->getCoreSystem(&coreSystem);
	if(result != FMOD_OK || coreSystem == nullptr){
		Debug::ThrowFatalError(SID("AUDIO"), "Could not get FMOD Core system! FMOD Error: " + std::string(FMOD_ErrorString(result)), ErrorCode::FMOD_Init_Error, __FILE__, __LINE__);
	}

	unsigned int version = 0;
	result = coreSystem->getVersion(&version);
	if(result != FMOD_OK){
		Debug::ThrowFatalError(SID("AUDIO"), "Could not get FMOD version! FMOD Error: " + std::string(FMOD_ErrorString(result)), ErrorCode::FMOD_Init_Error, __FILE__, __LINE__);
	}

	if(version != FMOD_VERSION){
		Debug::ThrowFatalError(SID("AUDIO"), "FMOD header version does not match FMOD lib version!", ErrorCode::FMOD_Version_Error, __FILE__, __LINE__);
	}

	result = studioSystem->initialize(4095, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK){
		Debug::ThrowFatalError(SID("AUDIO"), "FMOD Studio System could not be initialized! FMOD Error: " + std::string(FMOD_ErrorString(result)), ErrorCode::FMOD_Init_Error, __FILE__, __LINE__);
	}
}

Audio::~Audio(){
	if(studioSystem != nullptr){
		FMOD_RESULT result = studioSystem->release();
		if(result != FMOD_OK){
			Debug::Log(SID("AUDIO"), "FMOD Studio System could not be released! FMOD Error: " + std::string(FMOD_ErrorString(result)), Debug::Error, __FILE__, __LINE__);
		}

		studioSystem = nullptr;
		coreSystem = nullptr;
	}
}

void Audio::Update(){
	studioSystem->update();
	coreSystem->update();
}