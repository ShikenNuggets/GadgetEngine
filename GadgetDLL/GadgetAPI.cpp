//Export functions to the DLL to be used by the 

#ifndef WORKBENCH_INTERFACE
#define WORKBENCH_INTERFACE extern "C" __declspec(dllexport)
#endif //!WORKBENCH_INTERFACE

#include <Windows.h>

#include <App.h>

using namespace Gadget;

namespace{
	HMODULE gameCodeDLL{ nullptr };

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
		const char* name;
		//TODO - Array of strings for the tag
		TransformInfo transform;
	};
} //Anonymous Namespace

WORKBENCH_INTERFACE uint32_t LoadGameCodeDLL(const char* dllPath_){
	if(gameCodeDLL){
		return FALSE;
	}

	gameCodeDLL = LoadLibraryA(dllPath_);
	GADGET_BASIC_ASSERT(gameCodeDLL);

	return gameCodeDLL ? TRUE : FALSE;
}

WORKBENCH_INTERFACE uint32_t UnloadGameCodeDLL(){
	if(!gameCodeDLL){
		return FALSE;
	}

	GADGET_BASIC_ASSERT(gameCodeDLL);
	int result = FreeLibrary(gameCodeDLL);
	GADGET_BASIC_ASSERT(result);
	gameCodeDLL = nullptr;
	return TRUE;
}

WORKBENCH_INTERFACE bool InitForWorkbench(){
	//Call anything that needs to be setup before the editor uses it here
	Gadget::GUID::SetInitialGUID();
	return true;
}

WORKBENCH_INTERFACE uint64_t CreateGameObject(GameObjectDescriptor* descriptor_){
	GADGET_BASIC_ASSERT(descriptor_ != nullptr);

	GameObjectProperties properties = GameObjectProperties(Gadget::GUID::Invalid, StringID::ProcessString(descriptor_->name), std::vector<std::string>(), descriptor_->transform.ToTransform());
	GameObject* go = new GameObject(properties);
	return go->GetGUID().Id();
}

WORKBENCH_INTERFACE void DestroyGameObject(uint64_t guid_){
	GADGET_BASIC_ASSERT(guid_ != Gadget::GUID::Invalid);

	GameObject* go = GameObjectCollection::Get(guid_);
	GADGET_BASIC_ASSERT(go != nullptr);

	GameObjectCollection::Remove(guid_);
	delete go;
}