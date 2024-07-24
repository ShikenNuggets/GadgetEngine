#ifndef GADGET_AUDIO_AUDIO_LISTENER_H
#define GADGET_AUDIO_AUDIO_LISTENER_H

#include "Game/Component.h"
#include "Game/GameObject.h"

namespace Gadget{
	class AudioListener : public Component{
	public:
		AudioListener(GameObject* parent_);
		AudioListener(GUID parentGUID_);
		AudioListener(const ComponentProperties& props_);

		virtual ~AudioListener() override;

		static AudioListener* Get(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.Get(objectGuid_);
		}

		static std::vector<AudioListener*> GetComponents(GUID objectGuid_){
			GADGET_BASIC_ASSERT(objectGuid_ != GUID::Invalid);
			return componentCollection.GetComponents(objectGuid_);
		}

		void Update();

	private:
		static ComponentCollection<AudioListener> componentCollection;
	};
}

#endif //!GADGET_AUDIO_AUDIO_LISTENER_H