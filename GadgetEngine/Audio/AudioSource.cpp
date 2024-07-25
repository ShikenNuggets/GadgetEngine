#include "AudioSource.h"

#include <fmod_errors.h>

#include "App.h"
#include "Audio/FMODHelper.h"

using namespace Gadget;

ComponentCollection<AudioSource> AudioSource::componentCollection;

AudioSource::AudioSource(GameObject* parent_, StringID clipName_, SoundType type_, VolumeChannel volumeChannel_) : Component(SID("AudioSource"), parent_), soundType(type_), volumeChannel(volumeChannel_), clipName(clipName_), audioClip(nullptr), channel(nullptr){
	componentCollection.Add(this);

	audioClip = App::GetResourceManager().LoadResource<AudioClip>(clipName);
	GADGET_BASIC_ASSERT(audioClip != nullptr);
}

AudioSource::AudioSource(GUID parentGUID_, StringID clipName_, SoundType type_, VolumeChannel volumeChannel_) : Component(SID("AudioSource"), parentGUID_), soundType(type_), volumeChannel(volumeChannel_), clipName(clipName_), audioClip(nullptr), channel(nullptr){
	componentCollection.Add(this);

	audioClip = App::GetResourceManager().LoadResource<AudioClip>(clipName);
	GADGET_BASIC_ASSERT(audioClip != nullptr);
}

AudioSource::AudioSource(const ComponentProperties& props_) : Component(props_), soundType(SoundType::_2D), volumeChannel(VolumeChannel::Master), clipName(StringID::None), audioClip(nullptr), channel(nullptr){
	componentCollection.Add(this);
	Deserialize(props_);

	audioClip = App::GetResourceManager().LoadResource<AudioClip>(clipName);
	GADGET_BASIC_ASSERT(audioClip != nullptr);
}

AudioSource::~AudioSource(){
	componentCollection.Remove(this);

	if(audioClip != nullptr){
		App::GetResourceManager().UnloadResource(clipName);
		audioClip = nullptr;
	}
}

void AudioSource::Update(){
	Set3DAttributes();

	FMOD_RESULT result = channel->setVolume(App::GetAudio().GetVolume(volumeChannel));
	if(result != FMOD_OK){
		GADGET_LOG_ERROR(SID("AUDIO"), "Could not set channel audio! FMOD Error: " + std::string(FMOD_ErrorString(result)));
	}
}

ErrorCode AudioSource::Play(bool loop_){
	Stop();

	FMOD::System* coreSystem = App::GetAudio().GetCoreSystem();
	GADGET_BASIC_ASSERT(coreSystem != nullptr);

	FMOD::Sound* soundToPlay = (soundType == SoundType::_2D) ? audioClip->GetSound2D() : audioClip->GetSound3D();
	
	FMOD_RESULT result = coreSystem->playSound(soundToPlay, 0, true, &channel); //Sound starts paused so we can fix up positional info before it starts playing
	if(result != FMOD_OK || channel == nullptr){
		GADGET_LOG_ERROR(SID("AUDIO"), "Sound could not be played! FMOD Error: " + std::string(FMOD_ErrorString(result)));
		return ErrorCode::FMOD_Error;
	}

	ErrorCode err = Set3DAttributes();
	if(err != ErrorCode::OK){
		return err;
	}

	result = channel->setVolume(App::GetAudio().GetVolume(volumeChannel));
	if(result != FMOD_OK){
		GADGET_LOG_ERROR(SID("AUDIO"), "Could not set channel audio! FMOD Error: " + std::string(FMOD_ErrorString(result)));
		return ErrorCode::FMOD_Error;
	}

	FMOD_MODE mode = loop_ ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	result = channel->setMode(mode);
	if(result != FMOD_OK){
		GADGET_LOG_ERROR(SID("AUDIO"), "Could not set channel mode! FMOD Error: " + std::string(FMOD_ErrorString(result)));
		return ErrorCode::FMOD_Error;
	}

	result = channel->setPaused(false);
	if(result != FMOD_OK){
		GADGET_LOG_ERROR(SID("AUDIO"), "Could not unpause channel! FMOD Error: " + std::string(FMOD_ErrorString(result)));
		return ErrorCode::FMOD_Error;
	}

	return ErrorCode::OK;
}

ErrorCode AudioSource::Stop(){
	if(channel != nullptr){
		FMOD_RESULT result = channel->stop();
		if(result != FMOD_OK){
			GADGET_LOG_ERROR(SID("AUDIO"), "Could not stop channel! FMOD Error: " + std::string(FMOD_ErrorString(result)));
			return ErrorCode::FMOD_Error;
		}

		channel = nullptr;
	}

	return ErrorCode::OK;
}

ComponentProperties AudioSource::Serialize() const{
	ComponentProperties props = Component::Serialize();

	props.variables.Add(SID("SoundType"), static_cast<uint8_t>(soundType));
	props.variables.Add(SID("VolumeChannel"), static_cast<uint8_t>(volumeChannel));
	props.variables.Add(SID("ClipName"), clipName);

	return props;
}

void AudioSource::Deserialize(const ComponentProperties& props_){
	soundType = static_cast<SoundType>(props_.variables.GetValue(SID("SoundType"), 0).ToNumber<uint8_t>());
	volumeChannel = static_cast<VolumeChannel>(props_.variables.GetValue(SID("VolumeChannel"), 0).ToNumber<uint8_t>());
	clipName = props_.variables.GetValue(SID("ClipName"), SID("")).Name();

	GADGET_BASIC_ASSERT(static_cast<int>(soundType) >= 0);
	GADGET_BASIC_ASSERT(soundType < SoundType::SoundType_MAX);

	GADGET_BASIC_ASSERT(static_cast<int>(volumeChannel) >= 0);
	GADGET_BASIC_ASSERT(volumeChannel < VolumeChannel::VolumeChannel_MAX);
}

ErrorCode AudioSource::Set3DAttributes(){
	if(channel == nullptr || soundType == SoundType::_2D){
		return ErrorCode::OK;
	}

	Vector3 velocity = Vector3::Zero();
	Rigidbody* rb = Rigidbody::Get(parent->GetGUID());
	if(rb != nullptr){
		velocity = rb->GetVelocity();
	}

	const FMOD_VECTOR pos = FMODHelper::ConvertVector3(parent->GetPosition());
	const FMOD_VECTOR vel = FMODHelper::ConvertVector3(velocity);

	FMOD_RESULT result = channel->set3DAttributes(&pos, &vel);
	if(result != FMOD_OK){
		GADGET_LOG_ERROR(SID("AUDIO"), "Could not set 3D channel attributes! FMOD Error: " + std::string(FMOD_ErrorString(result)));
		return ErrorCode::FMOD_Error;
	}

	return ErrorCode::OK;
}