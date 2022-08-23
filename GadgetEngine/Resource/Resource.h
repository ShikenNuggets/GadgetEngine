#ifndef GADGET_RESOURCE_H
#define GADGET_RESOURCE_H

#include <typeinfo>

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

		virtual Resource* LoadResource() = 0;

		virtual void UnloadResource(){
			if(loadCount > 0){
				loadCount--;
			}

			if(loadCount == 0){
				delete resource;
				resource = nullptr;
			}
		}

		virtual const std::type_info& GetResourceTypeInfo() = 0;

	protected:
		Resource* resource;
		size_t loadCount;
	};
}

#endif //!GADGET_RESOURCE_H