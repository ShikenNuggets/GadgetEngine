#ifndef GADGET_CUBEMAP_H
#define GADGET_CUBEMAP_H

#include <array>

#include "Texture.h"
#include "Debug.h"
#include "Utils/StringID.h"

namespace Gadget{
	class Cubemap{
	public:
		enum Orientation{
			Right = 0,
			Left = 1,
			Top = 2,
			Bottom = 3,
			Back = 4,
			Front = 5
		};

		Cubemap(StringID right_, StringID left_, StringID top_, StringID bottom_, StringID back_, StringID front_);
		~Cubemap();

		bool IsValid() const;

		int GetFaceWidth() const{ GADGET_BASIC_ASSERT(IsValid()); return textures[0]->GetWidth(); }
		int GetFaceHeight() const{ GADGET_BASIC_ASSERT(IsValid()); return textures[0]->GetHeight(); }
		int GetBitDepth() const{ GADGET_BASIC_ASSERT(IsValid()); return textures[0]->GetBitDepth(); }

		Texture* GetTexture(Orientation o_) const;
		Texture* GetTexture(size_t orientation_) const;

	private:
		static constexpr int numTextures = 6;
		std::array<StringID, numTextures> textureNames;
		std::array<Texture*, numTextures> textures;
	};
}

#endif //!GADGET_CUBEMAP_H