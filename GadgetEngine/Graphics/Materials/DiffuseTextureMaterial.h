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
		DiffuseTextureMaterial(StringID textureResource_, StringID shaderResource_) : Material(shaderResource_), textureInfo(nullptr){
			GADGET_BASIC_ASSERT(textureResource_ != StringID::None);
			GADGET_BASIC_ASSERT(shaderResource_ != StringID::None);

			Texture* tex = App::GetResourceManager().LoadResource<Texture>(textureResource_);
			GADGET_ASSERT(tex != nullptr, "Could not load texture [" + textureResource_.GetString() + "]!");

			textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
			GADGET_ASSERT(textureInfo != nullptr, "Could not generate texture info!");
			App::GetResourceManager().UnloadResource(textureResource_);

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

		virtual bool HasLighting() const override{ return true; }

	private:
		TextureInfo* textureInfo;
	};
}

#endif //!GADGET_DIFFUSE_TEXTURE_MATERIAL_H