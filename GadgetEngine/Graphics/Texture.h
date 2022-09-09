#ifndef GADGET_TEXTURE_H
#define GADGET_TEXTURE_H

#include <vector>

namespace Gadget{
	class Texture{
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
}

#endif //!GADGET_TEXTURE_H