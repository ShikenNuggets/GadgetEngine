#ifndef STARHAWX_PROJECTILE_OBJECT_H
#define STARHAWX_PROJECTILE_OBJECT_H

#include <Game/GameObject.h>

#include "ProjectileController.h"

namespace StarHawx{
	class ProjectileObject : public Gadget::GameObject{
	public:
		ProjectileObject(const Gadget::Vector3 position_, const Gadget::Quaternion rotation_) : GameObject(SID("Projectile")){
			SetPosition(position_);
			SetRotation(rotation_);
			SetScale(0.35f, 0.35f, 2.5f);
			SetLifeTime(10.0f);

			AddComponent(new Gadget::RenderComponent(GetGUID(), SID("CubeModel"), SID("RedMaterial")));
			AddComponent(new Gadget::Rigidbody(this, 0.01f, false, Gadget::FreezeRotationType::None));
			AddComponent(new Gadget::CubeCollider(this));
			AddComponent(new Gadget::AudioSource(this, SID("LazerSFX"), Gadget::SoundType::_2D, Gadget::VolumeChannel::SFX, Gadget::SoundPlayMode::PlayOnce));
			AddComponent(new ProjectileController(this));
		}
	};
}

#endif //!STARHAWX_PROJECTILE_OBJECT_H