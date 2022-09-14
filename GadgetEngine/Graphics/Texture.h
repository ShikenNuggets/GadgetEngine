#ifndef GADGET_TEXTURE_H
#define GADGET_TEXTURE_H

#include <vector>

#include "Loaders/BmpLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Texture : public Resource{
	public:
		Texture(int width_, int height_, int bitDepth_, const std::vector<uint8_t>& pixelData_) : width(width_), height(height_), bitDepth(bitDepth_), pixelData(pixelData_){}

		int GetWidth() const{ return width; }
		int GetHeight() const{ return width; }
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
		TextureResourceContainer(const std::string& path_) : path(path_){}

		virtual Resource* LoadResource() override{
			resource = BmpLoader::LoadImage(path);
			loadCount++;
			return resource;
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Texture);
		}

	private:
		std::string path;
	};
}

#endif //!GADGET_TEXTURE_H