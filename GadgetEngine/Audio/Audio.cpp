#include "Audio.h"

#include <fmod_errors.h>

#include "App.h"
#include "Debug.h"
#include "GadgetEnums.h"
#include "Audio/AudioListener.h"

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

void Audio::Update(Scene* scene_){
	const auto als = scene_->GetAllComponentsInScene<AudioListener>(); //TODO - This is slow
	for(const auto& al : als){
		al->Update();
	}

	studioSystem->update();
	coreSystem->update();
}

float Audio::GetVolume(VolumeChannel channel_){
	double masterVolume = App::GetConfig().GetOptionFloat(EngineVars::Audio::masterVolumeKey);
	double channelVolume = 1.0;

	switch(channel_){
		case VolumeChannel::Master:
			break;
		case VolumeChannel::Music:
			channelVolume = App::GetConfig().GetOptionFloat(EngineVars::Audio::musicVolumeKey);
			break;
		case VolumeChannel::Dialogue:
			channelVolume = App::GetConfig().GetOptionFloat(EngineVars::Audio::dialogueVolumeKey);
			break;
		case VolumeChannel::SFX:
			channelVolume = App::GetConfig().GetOptionFloat(EngineVars::Audio::sfxVolumeKey);
			break;
		case VolumeChannel::Ambience:
			channelVolume = App::GetConfig().GetOptionFloat(EngineVars::Audio::ambientVolumeKey);
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}

	return static_cast<float>(masterVolume * channelVolume);
}

void Audio::SetVolume(float volume_, VolumeChannel channel_){
	switch(channel_){
		case VolumeChannel::Master:
			App::GetConfig().SetOption(EngineVars::Audio::sectionName, EngineVars::Audio::masterVolumeKey, Math::Clamp(0.0f, 1.0f, volume_));
			break;
		case VolumeChannel::Music:
			App::GetConfig().SetOption(EngineVars::Audio::sectionName, EngineVars::Audio::musicVolumeKey, Math::Clamp(0.0f, 1.0f, volume_));
			break;
		case VolumeChannel::Dialogue:
			App::GetConfig().SetOption(EngineVars::Audio::sectionName, EngineVars::Audio::dialogueVolumeKey, Math::Clamp(0.0f, 1.0f, volume_));
			break;
		case VolumeChannel::SFX:
			App::GetConfig().SetOption(EngineVars::Audio::sectionName, EngineVars::Audio::sfxVolumeKey, Math::Clamp(0.0f, 1.0f, volume_));
			break;
		case VolumeChannel::Ambience:
			App::GetConfig().SetOption(EngineVars::Audio::sectionName, EngineVars::Audio::ambientVolumeKey, Math::Clamp(0.0f, 1.0f, volume_));
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}
}