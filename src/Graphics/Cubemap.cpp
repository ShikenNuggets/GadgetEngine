#include "Cubemap.h"

#include "App.h"
#include "Resource/ResourceManager.h"

using namespace Gadget;

Cubemap::Cubemap(StringID right_, StringID left_, StringID top_, StringID bottom_, StringID front_, StringID back_) : textureNames({ right_, left_, top_, bottom_, front_, back_ }), textures(){
	GADGET_BASIC_ASSERT(right_ != StringID::None);
	GADGET_BASIC_ASSERT(left_ != StringID::None);
	GADGET_BASIC_ASSERT(top_ != StringID::None);
	GADGET_BASIC_ASSERT(bottom_ != StringID::None);
	GADGET_BASIC_ASSERT(front_ != StringID::None);
	GADGET_BASIC_ASSERT(back_ != StringID::None);
	
	for(size_t i = 0; i < numTextures; i++){
		textures[i] = App::GetResourceManager().LoadResource<Texture>(textureNames[i]);
		GADGET_ASSERT(textures[i] != nullptr, "An error occurred while trying to load texture [" + textureNames[i].GetString() + "] for cubemap!");
	}
}

Cubemap::~Cubemap(){
	for(size_t i = 0; i < numTextures; i++){
		App::GetResourceManager().UnloadResource(textureNames[i]);
	}
}

bool Cubemap::IsValid() const{
	GADGET_ASSERT(textures[0] != nullptr, "Cubemap texture 0 was nullptr!");
	if(textures[0] == nullptr){
		return false;
	}

	const int expectedWidth = textures[0]->GetWidth();
	const int expectedHeight = textures[0]->GetHeight();
	const int expectedBitDepth = textures[0]->GetBitDepth();

	for(size_t i = 0; i < numTextures; i++){
		if(textures[i] == nullptr){
			Debug::Log(SID("RENDER"), "Cubemap Texture " + std::to_string(i) + " is null!", Debug::Error, __FILE__, __LINE__);
			return false;
		}

		if(textures[i]->GetWidth() != expectedWidth){
			Debug::Log(SID("RENDER"), "Cubemap Texture " + std::to_string(i) + " has the wrong width! Expected Value: " + std::to_string(expectedWidth) + ", Real Value: " + std::to_string(textures[i]->GetWidth()), Debug::Error, __FILE__, __LINE__);
			return false;
		}

		if(textures[i]->GetHeight() != expectedHeight){
			Debug::Log(SID("RENDER"), "Cubemap Texture " + std::to_string(i) + " has the wrong height! Expected Value: " + std::to_string(expectedHeight) + ", Real Value: " + std::to_string(textures[i]->GetHeight()), Debug::Error, __FILE__, __LINE__);
			return false;
		}

		if(textures[i]->GetBitDepth() != expectedBitDepth){
			Debug::Log(SID("RENDER"), "Cubemap Texture " + std::to_string(i) + " has the wrong bit depth! Expected Value: " + std::to_string(expectedBitDepth) + ", Real Value: " + std::to_string(textures[i]->GetBitDepth()), Debug::Error, __FILE__, __LINE__);
			return false;
		}
	}

	return true;
}

Texture* Cubemap::GetTexture(Orientation o_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_ASSERT(static_cast<int>(o_) >= 0 && static_cast<size_t>(o_) < numTextures, "Invalid Cubemap texture orientation [" + std::to_string(o_) + "]!");
	return textures[static_cast<size_t>(o_)];
}

Texture* Cubemap::GetTexture(size_t orientation_) const{
	GADGET_BASIC_ASSERT(IsValid());
	GADGET_BASIC_ASSERT(orientation_ < numTextures);
	return textures[orientation_];
}