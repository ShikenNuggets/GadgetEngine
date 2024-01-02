#ifndef GADGET_DIFFUSE_TEXTURE_MATERIAL_H
#define GADGET_DIFFUSE_TEXTURE_MATERIAL_H

#include "Material.h"
#include "App.h"
#include "Graphics/TextureInfo.h"
#include "Resource/ResourceManager.h"
#include "Utils/StringID.h"

namespace Gadget{
	class DiffuseTextureMaterial : public Material{
	public:
		static const StringID type;

		DiffuseTextureMaterial(StringID textureResource_, StringID shaderResource_) : Material(shaderResource_), textureResourceName(textureResource_), textureInfo(nullptr){
			GADGET_BASIC_ASSERT(textureResource_ != StringID::None);
			GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);

			Texture* tex = App::GetResourceManager().LoadResource<Texture>(textureResourceName);
			GADGET_ASSERT(tex != nullptr, "Could not load texture [" + textureResourceName.GetString() + "]!");

			textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
			GADGET_ASSERT(textureInfo != nullptr, "Could not generate texture info!");
			App::GetResourceManager().UnloadResource(textureResourceName);

			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);
		}

		~DiffuseTextureMaterial(){
			delete textureInfo;
		}

		virtual void Bind() override{
			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);

			shader->Bind();
			textureInfo->Bind();
		}

		virtual void Unbind() override{
			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);

			textureInfo->Unbind();
			shader->Unbind();
		}

		virtual StringID TextureResourceName() const{ return textureResourceName; }
		virtual bool HasLighting() const override{ return true; }

		virtual StringID Type() const override{ return type; }

	private:
		StringID textureResourceName;
		TextureInfo* textureInfo;
	};
}

#endif //!GADGET_DIFFUSE_TEXTURE_MATERIAL_H