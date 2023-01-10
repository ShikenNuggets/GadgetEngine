#ifndef GADGET_RESOURCE_H
#define GADGET_RESOURCE_H

#include <typeinfo>

#include "Debug.h"

namespace Gadget{
	class Resource{
	public:
		virtual ~Resource(){}
	};

	class ResourceContainer{
	public:
		ResourceContainer() : resource(nullptr), loadCount(0){}
		virtual ~ResourceContainer(){ delete resource; }

		bool IsLoaded(){ return resource != nullptr; }

		virtual Resource* AddReference(){
			if(loadCount == 0){
				GADGET_ASSERT(resource == nullptr, "Loaded resource has a reference count of 0!");
				resource = LoadResource();
			}

			GADGET_BASIC_ASSERT(resource != nullptr);
			loadCount++;
			return resource;
		}

		virtual void UnloadResource(){
			if(loadCount > 0){
				loadCount--;
			}

			if(loadCount == 0){
				delete resource;
				resource = nullptr;
			}
		}

		//Physically load the actual resource, and NOTHING ELSE! Do not pass go, do not collect $200, do not manage memory
		virtual Resource* LoadResource() = 0;

		virtual const std::type_info& GetResourceTypeInfo() = 0;

	private:
		Resource* resource;
		size_t loadCount;
	};
}

#endif //!GADGET_RESOURCE_H