//Export functions to the DLL to be used by the editor

#ifndef WORKBENCH_INTERFACE
#define WORKBENCH_INTERFACE extern "C" __declspec(dllexport)
#endif //!WORKBENCH_INTERFACE

#include <Windows.h>

#include <App.h>
#include <Game/ComponentFactory.h>

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
	GADGET_ASSERT(gameCodeDLL, std::string("An error occurred while loading ") + dllPath_);

	return gameCodeDLL ? TRUE : FALSE;
}

WORKBENCH_INTERFACE uint32_t UnloadGameCodeDLL(){
	if(!gameCodeDLL){
		return FALSE;
	}

	GADGET_ASSERT(gameCodeDLL, "Tried unloading game code DLL that's not currently loaded");
	int result = FreeLibrary(gameCodeDLL);
	GADGET_ASSERT(result, "An error occurred while unloading game code DLL. Error Code: " + std::to_string(result));
	gameCodeDLL = nullptr;
	return TRUE;
}

WORKBENCH_INTERFACE bool InitForWorkbench(){
	//Call anything that needs to be setup before the editor uses it here
	Gadget::GUID::SetInitialGUID();
	Gadget::ComponentFactory::Init();
	return true;
}

WORKBENCH_INTERFACE uint64_t CreateGameObject(GameObjectDescriptor* descriptor_){
	GADGET_ASSERT(descriptor_ != nullptr, "GameObject descriptor must be a valid pointer");

	GameObjectProperties properties = GameObjectProperties(Gadget::GUID::Invalid, StringID::ProcessString(descriptor_->name), std::vector<std::string>(), descriptor_->transform.ToTransform());
	GameObject* go = new GameObject(properties);
	return go->GetGUID().Id();
}

WORKBENCH_INTERFACE void DestroyGameObject(uint64_t guid_){
	GADGET_ASSERT(guid_ != Gadget::GUID::Invalid, "GUID must be valid");

	GameObject* go = GameObjectCollection::Get(guid_);
	GADGET_ASSERT(go != nullptr, "GameObject with GUID " + std::to_string(guid_) + " does not exist");

	GameObjectCollection::Remove(guid_);
	delete go;
}

WORKBENCH_INTERFACE uint64_t GetStringLengthFromID(uint64_t id_){
	return StringID::GetStringFromID(id_).size();
}

WORKBENCH_INTERFACE void GetStringFromID(uint64_t id_, char* str_, uint64_t length_){
	std::string finalStr = StringID::GetStringFromID(id_);
	GADGET_ASSERT(finalStr.size() <= length_, "String is too long, will not fit in allocated buffer");

	for(uint64_t i = 0; i < finalStr.size(); i++){
		str_[i] = finalStr[i];
	}
}

WORKBENCH_INTERFACE uint64_t GetNumDeclaredComponents(){
	return ComponentFactory::GetAllDeclaredComponents().size();
}

WORKBENCH_INTERFACE void GetDeclaredComponents(uint64_t* namedVarArray){
	const auto decl = ComponentFactory::GetAllDeclaredComponents();

	for(uint64_t i = 0; i < decl.size(); i++){
		namedVarArray[i] = decl[i].GetID();
	}
}

WORKBENCH_INTERFACE uint64_t CreateComponentOfType(const char* type_, uint64_t parentObjectGuid_){
	GADGET_ASSERT(type_ != nullptr && std::string(type_) != "", "Type name is not a valid string");
	GADGET_ASSERT(parentObjectGuid_ != Gadget::GUID::Invalid, "Parent GameObject GUID must be valid");

	GameObject* parent = GameObjectCollection::Get(parentObjectGuid_);
	GADGET_ASSERT(parent != nullptr, "GameObject with GUID " + std::to_string(parentObjectGuid_) + " does not exist");

	StringID typeName = StringID::ProcessString(type_);
	Component* comp = ComponentFactory::InstantiateComponentType(typeName, ComponentProperties(typeName, Gadget::GUID::Invalid, parentObjectGuid_));
	GADGET_ASSERT(comp != nullptr, "An error occurred while instantiating the component type");

	parent->AddComponent(comp);

	return comp->GetGUID().Id();
}

WORKBENCH_INTERFACE uint64_t GetComponentTypeName(uint64_t componentGuid_, uint64_t parentGuid_){
	GADGET_ASSERT(componentGuid_ != Gadget::GUID::Invalid, "Component GUID must be valid");
	GADGET_ASSERT(parentGuid_ != Gadget::GUID::Invalid, "Parent GameObject GUID must be valid");

	GameObject* parent = GameObjectCollection::Get(parentGuid_);
	GADGET_ASSERT(parent != nullptr, "GameObject with GUID " + std::to_string(parentGuid_) + " does not exist");

	Component* component = parent->GetComponent(componentGuid_);
	GADGET_ASSERT(component != nullptr, "Component with GUID " + std::to_string(componentGuid_) + " does not exist on GameObject with GUID " + std::to_string(parentGuid_));
	GADGET_ASSERT(component->GetType() != StringID::None, "Invalid StringID for type name on component with GUID " + std::to_string(componentGuid_));

	return component->GetType().GetID();
}

WORKBENCH_INTERFACE bool GetComponentIsActivated(uint64_t componentGuid_, uint64_t parentGuid_){
	GADGET_ASSERT(componentGuid_ != Gadget::GUID::Invalid, "Component GUID must be valid");
	GADGET_ASSERT(parentGuid_ != Gadget::GUID::Invalid, "Parent GameObject GUID must be valid");

	GameObject* parent = GameObjectCollection::Get(parentGuid_);
	GADGET_ASSERT(parent != nullptr, "GameObject with GUID " + std::to_string(parentGuid_) + " does not exist");

	Component* component = parent->GetComponent(componentGuid_);
	GADGET_ASSERT(component != nullptr, "Component with GUID " + std::to_string(componentGuid_) + " does not exist on GameObject with GUID " + std::to_string(parentGuid_));

	return component->IsActivated();
}

WORKBENCH_INTERFACE uint64_t GetNumComponentProperties(uint64_t componentGuid_, uint64_t parentGuid_){
	GADGET_ASSERT(componentGuid_ != Gadget::GUID::Invalid, "Component GUID must be valid");
	GADGET_ASSERT(parentGuid_ != Gadget::GUID::Invalid, "Parent GameObject GUID must be valid");

	GameObject* parent = GameObjectCollection::Get(parentGuid_);
	GADGET_ASSERT(parent != nullptr, "GameObject with GUID " + std::to_string(parentGuid_) + " does not exist");

	Component* component = parent->GetComponent(componentGuid_);
	GADGET_ASSERT(component != nullptr, "Component with GUID " + std::to_string(componentGuid_) + " does not exist on GameObject with GUID " + std::to_string(parentGuid_));

	return component->Serialize().variables.Size();
}

WORKBENCH_INTERFACE void GetComponentProperties(uint64_t componentGuid_, uint64_t parentGuid_, NamedVar* namedVars){
	GADGET_ASSERT(componentGuid_ != Gadget::GUID::Invalid, "Component GUID must be valid");
	GADGET_ASSERT(parentGuid_ != Gadget::GUID::Invalid, "Parent GameObject GUID must be valid");
	GADGET_ASSERT(namedVars != nullptr, "NamedVars* was null");

	GameObject* parent = GameObjectCollection::Get(parentGuid_);
	GADGET_ASSERT(parent != nullptr, "GameObject with GUID " + std::to_string(parentGuid_) + " does not exist");

	Component* component = parent->GetComponent(componentGuid_);
	GADGET_ASSERT(component != nullptr, "Component with GUID " + std::to_string(componentGuid_) + " does not exist on GameObject with GUID " + std::to_string(parentGuid_));

	ComponentProperties props = component->Serialize();
	for(uint64_t i = 0; i < props.variables.Size(); i++){
		namedVars[i] = props.variables[i];
	}
}