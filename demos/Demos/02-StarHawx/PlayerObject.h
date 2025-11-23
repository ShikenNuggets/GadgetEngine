#ifndef STAR_HAWX_PLAYER_OBJECT_H
#define STAR_HAWX_PLAYER_OBJECT_H

#include <Game/GameObject.h>

#include "PlayerController.h"

namespace StarHawx{
	class PlayerObject : public Gadget::GameObject{
		PlayerObject(){
			std::vector<Gadget::StringID> materialNames;
			materialNames.push_back(SID("ArwingMaterial15"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial7"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial13"));
			materialNames.push_back(SID("ArwingMaterial10"));
			materialNames.push_back(SID("ArwingMaterial2"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial1"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial7"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial1"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial13"));
			materialNames.push_back(SID("ArwingMaterial3"));
			materialNames.push_back(SID("ArwingMaterial5"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial5"));
			materialNames.push_back(SID("ArwingMaterial15"));
			materialNames.push_back(SID("ArwingMaterial10"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial12"));
			materialNames.push_back(SID("ArwingMaterial6"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial14"));
			materialNames.push_back(SID("ArwingMaterial9"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial4"));
			materialNames.push_back(SID("ArwingMaterial11"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));
			materialNames.push_back(SID("ArwingMaterial8"));

			SetPosition(0.0f, 0.0f, -7.5f);
			SetRotation(0.0f, 180.0f, 0.0f);
			AddComponent(new Gadget::RenderComponent(GetGUID(), SID("ArwingModel"), materialNames));
			AddComponent(new Gadget::Rigidbody(this, 100.0f, false, Gadget::FreezeRotationType::FreezeAll));
			AddComponent(new Gadget::CubeCollider(this));
			AddComponent(new Gadget::AudioSource(this, SID("CorneriaMusic"), Gadget::SoundType::_2D, Gadget::VolumeChannel::Music, Gadget::SoundPlayMode::PlayLooping));
			AddComponent(new PlayerController(this));
		}
	};
}

#endif //!STAR_HAWX_PLAYER_OBJECT_H