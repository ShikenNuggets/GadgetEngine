//Export functions to the DLL to be used by the 

#ifndef WORKBENCH_INTERFACE
#define WORKBENCH_INTERFACE extern "C" __declspec(dllexport)
#endif //!WORKBENCH_INTERFACE

#include <App.h>

using namespace Gadget;

namespace{
	struct TransformInfo{
		float position[3];
		float rotation[3];
		float scale[3];

		Transform ToTransform(){
			return Transform(
				Vector3(position[0], position[1], position[2]),
				Euler(position[0], position[1], position[2]),
				Vector3(position[0], position[1], position[2])
			);
		}
	};

	struct GameObjectDescriptor{
		TransformInfo transform;
	};
} //Anonymous Namespace

WORKBENCH_INTERFACE uint64_t CreateGameObject(GameObjectDescriptor* descriptor_){
	GADGET_BASIC_ASSERT(descriptor_ != nullptr);

	Transform transform = descriptor_->transform.ToTransform();

	GameObject* go = new GameObject();
	go->SetPosition(transform.position);
	go->SetRotation(transform.rotation);
	go->SetScale(transform.scale);

	return go->GetGUID().Id();
}

WORKBENCH_INTERFACE void DestroyGameObject(uint64_t guid_){
	GADGET_BASIC_ASSERT(guid_ != GUID::Invalid);

	GameObject* go = GameObjectCollection::Get(guid_);
	GADGET_BASIC_ASSERT(go != nullptr);

	GameObjectCollection::Remove(guid_);
	delete go;
}