#ifndef GADGET_BINARY_BLOB_RESOURCE_H
#define GADGET_BINARY_BLOB_RESOURCE_H

#include "Resource.h"
#include "Core/FileSystem.h"

namespace Gadget{
	class BinaryBlobResource : public Resource{
	public:
		BinaryBlobResource(const std::vector<uint8_t>& blob_) : Resource() : blob(blob_){}
		virtual ~BinaryBlobResource() override{}

		static constexpr const char* typeName = "BinaryBlob";

	private:
		std::vector<uint8_t> blob;
	};

	class BinaryBlobResourceContainer : public ResourceContainer{
	public:
		BinaryBlobResourceContainer(const std::string& path_) : ResourceContainer(BinaryBlobResource::typeName, path_){
			GADGET_BASIC_ASSERT(!path_.empty());
			GADGET_BASIC_ASSERT(FileSystem::FileExists(path_));
		}

		virtual Resource* LoadResource() override{
			return new BinaryBlobResource(FileSystem::ReadBinaryFile(path));
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(BinaryBlobResource);
		}
	};
}

#endif //!GADGET_BINARY_BLOB_RESOURCE_H