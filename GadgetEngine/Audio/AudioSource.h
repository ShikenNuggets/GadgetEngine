#ifndef GADGET_AUDIO_AUDIO_SOURCE_H
#define GADGET_AUDIO_AUDIO_SOURCE_H

#include <fmod.hpp>

#include "GadgetEnums.h"
#include "Audio/AudioClip.h"
#include "Game/Component.h"

namespace Gadget{
	enum class SoundType : uint8_t{
		_2D,
		_3D,

		SoundType_MAX
	};

	class AudioSource : public Component{
	public:
		AudioSource(GameObject* parent_, StringID clipName_, SoundType type_, VolumeChannel volumeChannel_);
		AudioSource(GUID parentGUID_, StringID clipName_, SoundType type_, VolumeChannel volumeChannel_);
		AudioSource(const ComponentProperties& props_);

		virtual ~AudioSource() override;

		static AudioSource* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<AudioSource*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		void Update();

		ErrorCode Play(bool loop_ = false);
		ErrorCode Stop();

		virtual ComponentProperties Serialize() const override;

	protected:
		virtual void Deserialize(const ComponentProperties& props_) override;

	private:
		static ComponentCollection<AudioSource> componentCollection;

		SoundType soundType;
		VolumeChannel volumeChannel;
		StringID clipName;
		AudioClip* audioClip;
		FMOD::Channel* channel;

		ErrorCode Set3DAttributes();
	};
}

#endif //!GADGET_AUDIO_AUDIO_SOURCE_H