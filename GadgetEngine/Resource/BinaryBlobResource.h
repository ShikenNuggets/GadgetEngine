#ifndef GADGET_BINARY_BLOB_RESOURCE_H
#define GADGET_BINARY_BLOB_RESOURCE_H

#include "Core/FileSystem.h"
#include "Resource/Resource.h"

namespace Gadget{
	class BinaryBlobResource : public Resource{
	public:
		BinaryBlobResource(const std::vector<uint8_t>& blob_) : Resource(), blob(blob_){ blob.shrink_to_fit(); }
		virtual ~BinaryBlobResource() override{}

		static constexpr const char* typeName = "BinaryBlob";

		const std::vector<uint8_t>& Data() const{ return blob;  }
		std::vector<uint8_t>& Data(){ return blob; }
		size_t Size() const{ return blob.size(); }

	private:
		std::vector<uint8_t> blob;
	};

	class BinaryBlobResourceContainer : public ResourceContainer{
	public:
		BinaryBlobResourceContainer(const std::string& path_) : ResourceContainer(BinaryBlobResource::typeName, path_){}

		virtual Resource* LoadResource() override{
			return new BinaryBlobResource(FileSystem::ReadBinaryFile(path));
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(BinaryBlobResource);
		}
	};
}

#endif //!GADGET_BINARY_BLOB_RESOURCE_H