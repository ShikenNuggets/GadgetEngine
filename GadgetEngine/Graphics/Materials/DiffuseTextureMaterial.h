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
			Texture* tex = App::GetResourceManager().LoadResource<Texture>(textureResource_);
			GADGET_ASSERT(tex != nullptr, "Could not load texture [" + textureResource_.GetString() + "]!");

			textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
			GADGET_ASSERT(textureInfo != nullptr, "Could not generate texture info!");
			App::GetResourceManager().UnloadResource(textureResource_);
		}

		~DiffuseTextureMaterial(){
			delete textureInfo;
		}

		virtual void Bind() override{
			shader->Bind();
			textureInfo->Bind();
		}

		virtual void Unbind() override{
			textureInfo->Unbind();
			shader->Unbind();
		}

		virtual bool HasLighting() const override{ return true; }

	private:
		TextureInfo* textureInfo;
	};
}

#endif //!GADGET_DIFFUSE_TEXTURE_MATERIAL_H