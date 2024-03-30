#ifndef GADGET_RESOURCE_MANAGER_H
#define GADGET_RESOURCE_MANAGER_H

#include <map>

#include "Resource.h"
#include "Debug.h"
#include "Utils/StringID.h"

namespace Gadget{
	class ResourceManager{
	public:
		ResourceManager();
		~ResourceManager();

		template <class T> T* LoadResource(StringID resourceName_){
			static_assert(std::is_base_of<Resource, T>::value == true);
			GADGET_BASIC_ASSERT(resourceName_ != StringID::None);

			if(resources.find(resourceName_) == resources.end() || resources[resourceName_] == nullptr){
				Debug::Log(SID("RESOURCE"), "Attempted to load invalid resource [" + resourceName_.GetString() + "]", Debug::Warning, __FILE__, __LINE__);
				return nullptr;
			}

			if(resources[resourceName_]->GetResourceTypeInfo() != typeid(T)){
				Debug::Log(SID("RESOURCE"), "Attempted to load resource [" + resourceName_.GetString() + "] of type " + resources[resourceName_]->GetResourceTypeInfo().name() + " as a " + typeid(T).name(), Debug::Warning, __FILE__, __LINE__);
				return nullptr;
			}

			T* rs = dynamic_cast<T*>(resources[resourceName_]->AddReference());
			if(rs == nullptr){
				Debug::Log(SID("RESOURCE"), "Resource [" + resourceName_.GetString() + "] could not be loaded!", Debug::Error, __FILE__, __LINE__);
				resources[resourceName_]->RemoveReference(); //To fix the reference count
			}

			return rs;
		}

		void UnloadResource(StringID name_);

		void DeleteAllUnusedResources();

	private:
		std::map<StringID, ResourceContainer*> resources;

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	};
}

#endif //!GADGET_RESOURCE_MANAGER_H