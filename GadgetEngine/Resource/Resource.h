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
		ResourceContainer() : resource(nullptr), isLoaded(false), referenceCount(0){}
		virtual ~ResourceContainer(){ delete resource; }

		bool IsLoaded(){ return resource != nullptr; }
		size_t GetReferenceCount(){ return referenceCount; }

		virtual Resource* AddReference(){
			if(!isLoaded){
				GADGET_ASSERT(resource == nullptr, "Unloaded resource is not null!");
				resource = LoadResource();
				isLoaded = true;
			}

			GADGET_BASIC_ASSERT(resource != nullptr);
			referenceCount++;
			return resource;
		}

		void RemoveReference(){
			if(referenceCount > 0){
				referenceCount--;
			}
		}

		void DeleteIfUnused(){
			if(referenceCount == 0){
				delete resource;
				resource = nullptr;
				isLoaded = false;
			}
		}

		//Physically load the actual resource, and NOTHING ELSE! Do not pass go, do not collect $200, do not manage memory
		virtual Resource* LoadResource() = 0;

		virtual const std::type_info& GetResourceTypeInfo() = 0;

	private:
		Resource* resource;
		bool isLoaded;
		size_t referenceCount;
	};
}

#endif //!GADGET_RESOURCE_H