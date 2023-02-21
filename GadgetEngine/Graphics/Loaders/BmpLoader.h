#ifndef GADGET_BMP_LOADER_H
#define GADGET_BMP_LOADER_H

#include <string>
#include <vector>

namespace Gadget{
	//Forward declaration
	class Texture;

	struct BmpFileHeader{
		static constexpr uint16_t BMPType = 0x4D42;

		uint16_t fileType;
		uint32_t fileSize;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t offsetData;
	};

	struct BmpInfoHeader{
		uint32_t size;
		int32_t width;
		int32_t height;

		uint16_t planes;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t sizeImage;
		int32_t xPixelsPerMeter;
		int32_t yPixelsPerMeter;
		uint32_t colorsUsed;
		uint32_t colorsImportant;
	};

	struct BmpColorHeader{
		static constexpr uint32_t SRGBColorSpace = 0x73524742;
		static constexpr uint32_t RGBARedMask = 0x00ff0000;
		static constexpr uint32_t RGBAGreenMask = 0x0000ff00;
		static constexpr uint32_t RGBABlueMask = 0x000000ff;
		static constexpr uint32_t RGBAAlphaMask = 0xff000000;

		uint32_t redMask;
		uint32_t greenMask;
		uint32_t blueMask;
		uint32_t alphaMask;
		uint32_t colorSpaceType;
		uint32_t unused[16];
	};

	class BmpLoader{
	public:
		static Texture* LoadImage(const std::string& filePath_);

	private:
		static BmpFileHeader CreateFileHeader(const std::vector<uint8_t>& data_);
		static BmpInfoHeader CreateInfoHeader(const std::vector<uint8_t>& data_);
		static BmpColorHeader CreateColorHeader(const std::vector<uint8_t>& data_);
	};
}

#endif //!GADGET_BMP_LOADER_H