#include "BmpLoader.h"

#include "Debug.h"
#include "Core/FileSystem.h"
#include "Graphics/Texture.h"
#include "Utils/Utils.h"

using namespace Gadget;

Texture* BmpLoader::LoadImage(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	GADGET_BASIC_ASSERT(FileSystem::FileExists(filePath_));

	auto data = FileSystem::ReadBinaryFile(filePath_);
	data.shrink_to_fit();
	if(data.empty()){
		Debug::Log("Could not load BMP image file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	if(data.size() < sizeof(BmpFileHeader) + sizeof(BmpInfoHeader)){
		Debug::Log("Invalid format for BMP file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	ByteSequencer bs = ByteSequencer(data);

	BmpFileHeader fileHeader = CreateFileHeader(data, bs);

	if(fileHeader.fileType != BmpFileHeader::BMPType){
		Debug::Log("Invalid file type for BMP file [" + filePath_ + "]!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	BmpInfoHeader infoHeader = CreateInfoHeader(data, bs);
	if(infoHeader.compression != 0 && infoHeader.compression != 3){
		Debug::Log("Tried to load compressed BMP [" + filePath_ + "], but compression is not supported!", Debug::Error, __FILE__, __LINE__);
		return nullptr;
	}

	BmpColorHeader colorHeader;
	if(infoHeader.bitCount == 32){
		colorHeader = CreateColorHeader(data, bs);

		//We do not support other color spaces/formats. TODO?
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
BmpFileHeader BmpLoader::CreateFileHeader(const std::vector<uint8_t>& data_, ByteSequencer& byteSequencer_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader), "Invalid BMP data!");

	BmpFileHeader fileHeader{};
	fileHeader.fileType = byteSequencer_.Sequence2Bytes();
	fileHeader.fileSize = byteSequencer_.Sequence4Bytes();
	fileHeader.reserved1 = byteSequencer_.Sequence2Bytes();
	fileHeader.reserved2 = byteSequencer_.Sequence2Bytes();
	fileHeader.offsetData = byteSequencer_.Sequence4Bytes();

	return fileHeader;
}

BmpInfoHeader BmpLoader::CreateInfoHeader(const std::vector<uint8_t>& data_, ByteSequencer& byteSequencer_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader) + sizeof(BmpInfoHeader), "Invalid BMP data!");

	BmpInfoHeader infoHeader{};
	infoHeader.size = byteSequencer_.Sequence4Bytes();
	infoHeader.width = static_cast<int32_t>(byteSequencer_.Sequence4Bytes());
	infoHeader.height = static_cast<int32_t>(byteSequencer_.Sequence4Bytes());
	infoHeader.planes = byteSequencer_.Sequence2Bytes();
	infoHeader.bitCount = byteSequencer_.Sequence2Bytes();
	infoHeader.compression = byteSequencer_.Sequence4Bytes();
	infoHeader.sizeImage = static_cast<int32_t>(byteSequencer_.Sequence4Bytes());
	infoHeader.xPixelsPerMeter = static_cast<int32_t>(byteSequencer_.Sequence4Bytes());
	infoHeader.yPixelsPerMeter = static_cast<int32_t>(byteSequencer_.Sequence4Bytes());
	infoHeader.colorsUsed = byteSequencer_.Sequence4Bytes();
	infoHeader.colorsImportant = byteSequencer_.Sequence4Bytes();

	return infoHeader;
}

BmpColorHeader BmpLoader::CreateColorHeader(const std::vector<uint8_t>& data_, ByteSequencer& byteSequencer_){
	GADGET_ASSERT(data_.size() > sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + sizeof(BmpColorHeader), "Invalid BMP data!");

	BmpColorHeader colorHeader{};
	colorHeader.redMask = byteSequencer_.Sequence4Bytes();
	colorHeader.greenMask = byteSequencer_.Sequence4Bytes();
	colorHeader.blueMask = byteSequencer_.Sequence4Bytes();
	colorHeader.alphaMask = byteSequencer_.Sequence4Bytes();
	colorHeader.colorSpaceType = byteSequencer_.Sequence4Bytes();

	for(size_t i = 0; i < 16; i++){
		colorHeader.unused[i] = byteSequencer_.Sequence4Bytes();
	}

	return colorHeader;
}