#ifndef GADGET_RESOURCE_MANAGER_H
#define GADGET_RESOURCE_MANAGER_H

#include <map>

#include "Resource.h"
#include "Utils/StringID.h"

namespace Gadget{
	class ResourceManager{
	public:
		static ResourceManager* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		template <class T> T* LoadResource(StringID resourceName_){
			if(resources.find(resourceName_) == resources.end() || resources[resourceName_] == nullptr){
				Debug::Log(SID("RESOURCE"), "Attempted to load invalid resource [" + resourceName_.GetString() + "]", Debug::Warning, __FILE__, __LINE__);
				return nullptr;
			}

			if(resources[resourceName_]->GetResourceTypeInfo() != typeid(T)){
				Debug::Log(SID("RESOURCE"), "Attempted to load resource [" + resourceName_.GetString() + "] of type " + resources[resourceName_]->GetResourceTypeInfo().name() + " as a " + typeid(T).name(), Debug::Warning, __FILE__, __LINE__);
				return nullptr;
			}

			T* rs = dynamic_cast<T*>(resources[resourceName_]->LoadResource());
			if(rs == nullptr){
				Debug::Log(SID("RESOURCE"), "Resource [" + resourceName_.GetString() + "] could not be loaded!", Debug::Error, __FILE__, __LINE__);
				resources[resourceName_]->UnloadResource(); //To fix the reference count
			}

			return rs;
		}

		void UnloadResource(StringID name_);

	private:
		static ResourceManager* instance;

		ResourceManager();
		~ResourceManager();

		std::map<StringID, ResourceContainer*> resources;

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
	};
}

#endif //!GADGET_RESOURCE_MANAGER_H