#ifndef GADGET_TEXTURE_H
#define GADGET_TEXTURE_H

#include <vector>

#include "Debug.h"
#include "Loaders/BmpLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Texture : public Resource{
	public:
		Texture(int width_, int height_, int bitDepth_, const std::vector<uint8_t>& pixelData_) : width(width_), height(height_), bitDepth(bitDepth_), pixelData(pixelData_){}

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
			return BmpLoader::LoadImage(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Texture);
		}
	};
}

#endif //!GADGET_TEXTURE_H