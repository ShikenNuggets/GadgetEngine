#include "BmpLoader.h"

#include "Debug.h"
#include "Core/FileSystem.h"
#include "Graphics/Texture.h"

using namespace Gadget;

Texture* BmpLoader::LoadImage(const std::string& filePath_){
	auto data = FileSystem::ReadBinaryFile(filePath_);
	if(data.empty()){
		Debug::Log("Could not load BMP image file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	if(data.size() < sizeof(BmpFileHeader) + sizeof(BmpInfoHeader)){
		Debug::Log("Invalid format for BMP file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	BmpFileHeader fileHeader = CreateFileHeader(data);

	if(fileHeader.fileType != BmpFileHeader::BMPType){
		Debug::Log("Invalid file type for BMP file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	BmpInfoHeader infoHeader = CreateInfoHeader(data);
	if(infoHeader.compression != 0 && infoHeader.compression != 3){
		Debug::Log("Tried to load compressed BMP [" + filePath_ + "], but compression is not supported!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	BmpColorHeader colorHeader;
	if(infoHeader.bitCount == 32){
		colorHeader = CreateColorHeader(data);

		//We do not support other color spaces/formats. TODO?
		GADGET_ASSERT(colorHeader.colorSpaceType == BmpColorHeader::SRGBColorSpace, "Tried to load BMP with invalid color space (only SRGB is supported)!");
		GADGET_ASSERT(colorHeader.redMask == BmpColorHeader::RGBARedMask, "Tried to load BMP with invalid color format (only RGB and RGBA are supported)!");
		GADGET_ASSERT(colorHeader.greenMask == BmpColorHeader::RGBAGreenMask, "Tried to load BMP with invalid color format (only RGB and RGBA are supported)!");
		GADGET_ASSERT(colorHeader.blueMask == BmpColorHeader::RGBABlueMask, "Tried to load BMP with invalid color format (only RGB and RGBA are supported)!");
		GADGET_ASSERT(colorHeader.alphaMask == BmpColorHeader::RGBAAlphaMask, "Tried to load BMP with invalid color format (only RGB and RGBA are supported)!");
	}

	std::vector<uint8_t> pixelData;
	const size_t imgSize = static_cast<size_t>(infoHeader.width) * infoHeader.height * infoHeader.bitCount / 8;
	pixelData.reserve(imgSize);
	
	if(infoHeader.bitCount == 32){
		//Images are loaded in BGRA/BGRA
		//On Windows specifically, this is optimal
		for(size_t i = 0; i < imgSize; i += 4){
			pixelData.push_back(data[fileHeader.offsetData + i + 0]);
			pixelData.push_back(data[fileHeader.offsetData + i + 1]);
			pixelData.push_back(data[fileHeader.offsetData + i + 2]);
			pixelData.push_back(data[fileHeader.offsetData + i + 3]);
		}
	}else if(infoHeader.bitCount == 24){
		for(size_t i = 0; i < imgSize; i += 3){
			pixelData.push_back(data[fileHeader.offsetData + i + 0]);
			pixelData.push_back(data[fileHeader.offsetData + i + 1]);
			pixelData.push_back(data[fileHeader.offsetData + i + 2]);
		}
	}else{
		Debug::Log("Unsupported bitcount [" + std::to_string(infoHeader.bitCount) + " on BMP[" + filePath_ + "], but compression is not supported!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	return new Texture(infoHeader.width, infoHeader.height, infoHeader.bitCount, pixelData);
}

//TODO - These don't consider endianness at all, which could be a problem later
BmpFileHeader BmpLoader::CreateFileHeader(const std::vector<uint8_t>& data_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader), "Invalid BMP data!");

	BmpFileHeader fileHeader{};
	fileHeader.fileType = MergeBytes(data_[0], data_[1]);
	fileHeader.fileSize = MergeBytes(data_[2], data_[3], data_[4], data_[5]);
	fileHeader.reserved1 = MergeBytes(data_[6], data_[7]);
	fileHeader.reserved2 = MergeBytes(data_[8], data_[9]);
	fileHeader.offsetData = MergeBytes(data_[10], data_[11], data_[12], data_[13]);

	return fileHeader;
}

BmpInfoHeader BmpLoader::CreateInfoHeader(const std::vector<uint8_t>& data_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader) + sizeof(BmpInfoHeader), "Invalid BMP data!");

	BmpInfoHeader infoHeader{};
	infoHeader.size = MergeBytes(data_[14], data_[15], data_[16], data_[17]);
	infoHeader.width = static_cast<int32_t>(MergeBytes(data_[18], data_[19], data_[20], data_[21]));
	infoHeader.height = static_cast<int32_t>(MergeBytes(data_[22], data_[23], data_[24], data_[25]));
	infoHeader.planes = MergeBytes(data_[26], data_[27]);
	infoHeader.bitCount = MergeBytes(data_[28], data_[29]);
	infoHeader.compression = MergeBytes(data_[30], data_[31], data_[32], data_[33]);
	infoHeader.sizeImage = static_cast<int32_t>(MergeBytes(data_[34], data_[35], data_[36], data_[37]));
	infoHeader.xPixelsPerMeter = static_cast<int32_t>(MergeBytes(data_[38], data_[39], data_[40], data_[41]));
	infoHeader.yPixelsPerMeter = static_cast<int32_t>(MergeBytes(data_[42], data_[43], data_[44], data_[45]));
	infoHeader.colorsUsed = MergeBytes(data_[46], data_[47], data_[48], data_[49]);
	infoHeader.colorsImportant = MergeBytes(data_[50], data_[51], data_[52], data_[53]);

	return infoHeader;
}

BmpColorHeader BmpLoader::CreateColorHeader(const std::vector<uint8_t>& data_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + sizeof(BmpColorHeader), "Invalid BMP data!");

	BmpColorHeader colorHeader{};
	colorHeader.redMask = MergeBytes(data_[54], data_[55], data_[56], data_[57]);
	colorHeader.greenMask = MergeBytes(data_[58], data_[59], data_[60], data_[61]);
	colorHeader.blueMask = MergeBytes(data_[62], data_[63], data_[64], data_[65]);
	colorHeader.alphaMask = MergeBytes(data_[66], data_[67], data_[68], data_[69]);
	colorHeader.colorSpaceType = MergeBytes(data_[70], data_[71], data_[72], data_[73]);

	for(int i = 0; i < 16; i++){
		colorHeader.unused[i] = MergeBytes(data_[74 + (i * 4)], data_[75 + (i * 4)], data_[76 + (i * 4)], data_[77 + (i * 4)]);
	}

	return colorHeader;
}

//TODO - These don't consider endianness at all, which could be a problem later
uint16_t BmpLoader::MergeBytes(uint8_t a_, uint8_t b_){
	return (b_ << 8) | a_;
}

uint32_t BmpLoader::MergeBytes(uint8_t a_, uint8_t b_, uint8_t c_, uint8_t d_){
	return (d_ << 24) | (c_ << 16) | (b_ << 8) | a_;
}