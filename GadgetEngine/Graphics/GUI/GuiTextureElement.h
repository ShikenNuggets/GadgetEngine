#ifndef GADGET_GUI_TEXTURE_ELEMENT_H
#define GADGET_GUI_TEXTURE_ELEMENT_H

#include "GuiElement.h"
#include "Graphics/TextureInfo.h"
#include "App.h"

namespace Gadget{
	class GuiTextureElement : public GuiElement{
	public:
		GuiTextureElement(StringID name_, StringID textureResource_, const Vector2& pos_, const Vector2& size_, GuiAnchor anchor_, bool isActive_ = true) : GuiElement(name_, pos_, size_, anchor_, isActive_), textureName(textureResource_), shaderName(SID("Text2DShader")){
			GADGET_BASIC_ASSERT(name != StringID::None);
			GADGET_BASIC_ASSERT(textureResource_ != StringID::None);
			GADGET_BASIC_ASSERT(pos_.IsValid());
			GADGET_BASIC_ASSERT(size_.IsValid());
			GADGET_BASIC_ASSERT(anchor_ < GuiAnchor::GuiAnchor_MAX);
			
			auto* tex = App::GetResourceManager().LoadResource<Texture>(textureName);
			GADGET_BASIC_ASSERT(tex != nullptr);
			if(tex != nullptr){
				textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
			}else{
				textureInfo = nullptr;
			}

			meshInfo = App::GetRenderer().GenerateAPIDynamicMeshInfo(6, 4);
			shader = App::GetRenderer().GenerateAPIShader(shaderName);

			GADGET_BASIC_ASSERT(meshInfo != nullptr);
			GADGET_BASIC_ASSERT(shader != nullptr);
		}

		~GuiTextureElement(){
			delete textureInfo;
			App::GetResourceManager().UnloadResource(shaderName);
			App::GetResourceManager().UnloadResource(textureName);
		}

		Shader* GetShader() const{ return shader; }
		MeshInfo* GetMeshInfo() const{ return meshInfo; }
		TextureInfo* GetTextureInfo() const{ return textureInfo; }

	private:
		StringID textureName;
		StringID shaderName;
		Shader* shader;
		MeshInfo* meshInfo;
		TextureInfo* textureInfo;
	};
}

#endif //!GADGET_GUI_TEXTURE_H