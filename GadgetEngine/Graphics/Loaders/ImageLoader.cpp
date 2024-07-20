#include "ImageLoader.h"

#include "Graphics/Texture.h"

using namespace Gadget;

Texture* ImageLoader::LoadImage(const std::string& filePath_){
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	SDL_Surface* surface = IMG_Load(filePath_.c_str());
	GADGET_BASIC_ASSERT(surface != nullptr);
	if(surface == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "An error occurred on loading image " + filePath_);
		return nullptr;
	}

	std::vector<uint8_t> pixelData;
	for(size_t i = 0; i < static_cast<size_t>(surface->pitch) * surface->h; i++){
		pixelData.push_back(reinterpret_cast<uint8_t*>(surface->pixels)[i]);
	}

	return new Texture(surface->w, surface->h, surface->format->BytesPerPixel, pixelData);
}