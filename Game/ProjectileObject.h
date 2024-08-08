#ifndef STARHAWX_PROJECTILE_OBJECT_H
#define STARHAWX_PROJECTILE_OBJECT_H

#include <Game/GameObject.h>

#include "ProjectileController.h"

namespace StarHawx{
	class ProjectileObject : public Gadget::GameObject{
	public:
		ProjectileObject(){
			SetLifeTime(10.0f);
			SetScale(0.5f, 0.5f, 2.5f);
			AddComponent(new Gadget::RenderComponent(GetGUID(), SID("CubeModel"), SID("RedMaterial")));
			AddComponent(new Gadget::Rigidbody(this, 0.01f, false, Gadget::FreezeRotationType::None));
			AddComponent(new Gadget::CubeCollider(this));
			AddComponent(new ProjectileController(this));
		}
	};
}

#endif //!STARHAWX_PROJECTILE_OBJECT_H