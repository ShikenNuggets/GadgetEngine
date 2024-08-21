#ifndef GADGET_RESOURCE_H
#define GADGET_RESOURCE_H

#include "Debug.h"

namespace Gadget{
	class Resource{
	public:
		virtual ~Resource() = default;
	};

	class ResourceContainer{
	public:
		ResourceContainer(const std::string type_, const std::string& path_) : type(type_), path(path_), resource(nullptr), isLoaded(false), referenceCount(0){
			GADGET_BASIC_ASSERT(!type_.empty());
			GADGET_BASIC_ASSERT(!path_.empty());
		}

		virtual ~ResourceContainer(){ delete resource; }

		std::string Type() const{ return type; }
		std::string Path() const{ return path; }
		bool IsLoaded() const{ return resource != nullptr; }
		size_t GetReferenceCount() const{ return referenceCount; }

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

	protected:
		const std::string type;
		std::string path;

	private:
		Resource* resource;
		bool isLoaded;
		size_t referenceCount;
	};
}

#endif //!GADGET_RESOURCE_H