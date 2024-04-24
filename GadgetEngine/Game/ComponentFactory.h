#ifndef GADGET_COMPONENT_FACTORY_H
#define GADGET_COMPONENT_FACTORY_H

#include "Game/Component.h"
#include "Utils/StringID.h"

namespace Gadget{
	typedef std::function<Component* (const ComponentProperties&)> componentDeserializeFunc;

	class ComponentFactory{
	public:
		static void Init();

		static constexpr std::vector<StringID> GetAllDeclaredComponents(){
			return Utils::GetAllKeys(deserializers);
		}

		static constexpr void DeclareComponentDeserializer(StringID type_, const componentDeserializeFunc& deserializer_){
			GADGET_BASIC_ASSERT(type_ != StringID::None);
			
			if(!Utils::ContainsKey(deserializers, type_)){
				deserializers.emplace(type_, deserializer_);
			}
		}

		static Component* InstantiateComponentType(StringID type_, const ComponentProperties& props_){
			GADGET_BASIC_ASSERT(type_ != StringID::None);
			GADGET_ASSERT(Utils::ContainsKey(deserializers, type_), "Tried to instantiate component of unknown type [" + type_.GetString() + "]! Did you apply the macro to it?");
			return deserializers[type_](props_);
		}

	private:
		static std::map<StringID, componentDeserializeFunc> deserializers;
	};

	#define GADGET_DECLARE_SERIALIZABLE_COMPONENT(T) Gadget::ComponentFactory::DeclareComponentDeserializer(SID(#T), &Gadget::Component::DeserializeToNewComponent<T>)
}

#endif //!GADGET_COMPONENT_FACTORY_H