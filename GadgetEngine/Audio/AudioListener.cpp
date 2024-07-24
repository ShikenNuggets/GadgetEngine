#include "AudioListener.h"

#include "App.h"
#include "Audio/FMODHelper.h"

using namespace Gadget;

ComponentCollection<AudioListener> AudioListener::componentCollection;

AudioListener::AudioListener(GameObject* parent_) : Component(SID("AudioListener"), parent_){
	componentCollection.Add(this);

	if(componentCollection.Count() > 1){
		GADGET_LOG_WARNING(SID("AUDIO"), "Multiple audio listeners are present. This is currently not supported");
	}
}

AudioListener::AudioListener(GUID parentGUID_) : Component(SID("AudioListener"), parentGUID_){
	componentCollection.Add(this);

	if(componentCollection.Count() > 1){
		GADGET_LOG_WARNING(SID("AUDIO"), "Multiple audio listeners are present. This is currently not supported");
	}
}

AudioListener::AudioListener(const ComponentProperties& props_) : Component(props_){
	componentCollection.Add(this);

	if(componentCollection.Count() > 1){
		GADGET_LOG_WARNING(SID("AUDIO"), "Multiple audio listeners are present. This is currently not supported");
	}
}

AudioListener::~AudioListener(){
	componentCollection.Remove(this);
}

void AudioListener::Update(){
	GADGET_BASIC_ASSERT(parent != nullptr);

	FMOD::System* coreSystem = App::GetAudio().GetCoreSystem();
	GADGET_BASIC_ASSERT(coreSystem != nullptr);

	Vector3 velocity = Vector3::Zero();
	Rigidbody* rb = Rigidbody::Get(parent->GetGUID());
	if(rb != nullptr){
		velocity = rb->GetVelocity();
	}

	FMOD_VECTOR pos = FMODHelper::ConvertVector3(parent->GetPosition());
	FMOD_VECTOR vel = FMODHelper::ConvertVector3(velocity);
	FMOD_VECTOR forw = FMODHelper::ConvertVector3(parent->GetTransform().Forward());
	FMOD_VECTOR up = FMODHelper::ConvertVector3(parent->GetTransform().Up());

	coreSystem->set3DListenerAttributes(0, &pos, &vel, &forw, &up); //TODO - Support multiple listeners correctly
}