#ifndef GADGET_RESOURCE_H
#define GADGET_RESOURCE_H

#include "Debug.h"

namespace Gadget{
	class Resource{
	public:
		virtual ~Resource() = default;

		virtual size_t SizeInBytes() const = 0;
	};

	class ResourceContainer{
	public:
		ResourceContainer(const std::string& type_, const std::string& path_) : type(type_), path(path_), resource(nullptr), isLoaded(false), referenceCount(0){
			GADGET_BASIC_ASSERT(!type_.empty());
			GADGET_BASIC_ASSERT(!path_.empty());
			GADGET_ASSERT_FILE_EXISTS(path_);
		}

		virtual ~ResourceContainer(){ delete resource; }

		std::string Type() const{ return type; }
		std::string Path() const{ return path; }
		bool IsLoaded() const{ return resource != nullptr; }
		size_t GetReferenceCount() const{ return referenceCount; }

		size_t ResourceSizeInBytes() const{
			if(!isLoaded){
				return 0;
			}

			GADGET_BASIC_ASSERT(resource != nullptr);
			if(resource != nullptr){
				return resource->SizeInBytes();
			}

			GADGET_ASSERT_UNREACHABLE;
			return 0;
		}

		virtual Resource* AddReference(){
			if(!isLoaded){
				GADGET_ASSERT(resource == nullptr, "Unloaded resource is not null!");
				resource = LoadResource();
				if(resource != nullptr){
					Debug::Log(SID("RESOURCE"), type + " [" + FileSystem::GetFileNameFromPath(path) + "] is using " + Utils::FormatToByteSizeString(resource->SizeInBytes()), Debug::Verbose);
				}else{
					GADGET_LOG_WARNING(SID("RESOURCE"), "Attempting to load " + type + " [" + FileSystem::GetFileNameFromPath(path) + "] returned nullptr!");
				}

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