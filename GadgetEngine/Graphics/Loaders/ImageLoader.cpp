#include "ImageLoader.h"

#include "Graphics/Texture.h"

using namespace Gadget;

Texture* ImageLoader::LoadImage(const std::string& filePath_){
	//static constexpr int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	//const int init = IMG_Init(flags);
	//if((init & flags) != flags){
	//	GADGET_ASSERT(false, std::string("SDL_Image init failed. SDL Error: ") + SDL_GetError());
	//	return nullptr;
	//}

	SDL_Surface* surface = IMG_Load(filePath_.c_str());
	GADGET_BASIC_ASSERT(surface != nullptr);
	if(surface == nullptr || surface->pixels == nullptr){
		GADGET_LOG_WARNING(SID("RENDER"), "An error occurred on loading " + filePath_ + ". SDL Error: " + SDL_GetError());
		return nullptr;
	}

	const size_t numPixels = static_cast<size_t>(surface->pitch) * surface->h;

	std::vector<uint8_t> pixelData;
	pixelData.reserve(numPixels);
	for(size_t i = 0; i < numPixels; i++){
		pixelData.push_back(reinterpret_cast<uint8_t*>(surface->pixels)[i]);
	}

    const auto* formatDetails = SDL_GetPixelFormatDetails(surface->format);
	Texture* texture = new Texture(surface->w, surface->h, formatDetails->bits_per_pixel, pixelData);
	SDL_DestroySurface(surface);
	return texture;
}