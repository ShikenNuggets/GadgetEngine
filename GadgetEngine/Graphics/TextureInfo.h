#ifndef GADGET_TEXTURE_INFO_H
#define GADGET_TEXTURE_INFO_H

#include "Texture.h"

namespace Gadget{
	class TextureInfo{
	public:
		TextureInfo(){}
		virtual ~TextureInfo(){}

		virtual void Bind(int textureIndex_ = 0) = 0;
		virtual void Unbind() = 0;
	};
}

#endif //!GADGET_TEXTURE_INFO_HJ