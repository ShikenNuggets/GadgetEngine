#ifndef GADGET_RESOURCE_MANAGER_H
#define GADGET_RESOURCE_MANAGER_H

#include <map>

#include "Debug.h"
#include "Data/Queue.h"
#include "Resource/Resource.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"

namespace Gadget{
	class ResourceManager{
	public:
		static inline constexpr const char* gResourcesJsonFile = "Resources\\resources.json";

		ResourceManager();
		~ResourceManager();
		DISABLE_COPY_AND_MOVE(ResourceManager);

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

		size_t GetMemoryUsageOfAllResources() const;
		size_t GetMemoryUsageOfUnusedResources() const;

		void SetUnusedResourceMemoryBudget(size_t max_){
			maxUnusedResourceMemory = max_;
		}

		void Cleanup(); // (gradually) delete (some) unused resources
		void ForceDeleteAllUnusedResources();

		template <class T>
		void ForceUnloadAssetsOfType(){
			static_assert(std::is_base_of_v<Resource, T>);
			for(auto& r : resources){
				if(r.second->GetResourceTypeInfo() == typeid(T)){
					r.second->DeleteIfUnused();
				}
			}
		}

	private:
		std::map<StringID, ResourceContainer*> resources;
		Queue<StringID> resourcesToUnload;
		size_t maxUnusedResourceMemory;
	};
}

#endif //!GADGET_RESOURCE_MANAGER_H