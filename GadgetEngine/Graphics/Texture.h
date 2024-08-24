#ifndef GADGET_TEXTURE_H
#define GADGET_TEXTURE_H

#include <vector>

#include "Debug.h"
#include "Loaders/BmpLoader.h"
#include "Loaders/ImageLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Texture : public Resource{
	public:
		Texture(int width_, int height_, int bitDepth_, const std::vector<uint8_t>& pixelData_) : width(width_), height(height_), bitDepth(bitDepth_), pixelData(pixelData_){
			GADGET_BASIC_ASSERT(width_ > 0);
			GADGET_BASIC_ASSERT(height_ > 0);
			GADGET_BASIC_ASSERT(!pixelData_.empty());
			GADGET_ASSERT(bitDepth >= 24, "Bit depth of less than 24 is not supported, tried to load with bit depth of " + std::to_string(bitDepth));
			GADGET_ASSERT(bitDepth <= 32, "Bit depth of greater than 32 is not supported, tried to load with bit depth of " + std::to_string(bitDepth));
		}

		static constexpr const char* typeName = "Texture";

		int GetWidth() const{ return width; }
		int GetHeight() const{ return height; }
		int GetBitDepth() const{ return bitDepth; }
		const std::vector<uint8_t>& GetPixels() const{ return pixelData; }

	private:
		const int width;
		const int height;
		const int bitDepth;
		const std::vector<uint8_t> pixelData;
	};

	class TextureResourceContainer : public ResourceContainer{
	public:
		TextureResourceContainer(const std::string& path_) : ResourceContainer(Texture::typeName, path_){}

		std::string GetPath() const{ return path; }

		virtual Resource* LoadResource() override{
			return ImageLoader::LoadImage(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Texture);
		}
	};
}

#endif //!GADGET_TEXTURE_H