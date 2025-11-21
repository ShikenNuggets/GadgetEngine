#ifndef GADGET_GRAPHICS_LOADERS_IMAGE_LOADER_H
#define GADGET_GRAPHICS_LOADERS_IMAGE_LOADER_H

#include <string>

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26819) //Unnanotated fallthrough in switch statement
#include <SDL_image.h>
#pragma warning(default : 26819)

namespace Gadget{
	//Forward declaration
	class Texture;

	class ImageLoader{
	public:
		static Texture* LoadImage(const std::string& filePath_);
	};
}

#endif //!GADGET_GRAPHICS_LOADERS_IMAGE_LOADER_H