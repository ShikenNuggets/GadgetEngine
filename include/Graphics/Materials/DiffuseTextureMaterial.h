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

			if(tex != nullptr){
				textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
				GADGET_ASSERT(textureInfo != nullptr, "Could not generate texture info!");
			}
			
			App::GetResourceManager().UnloadResource(textureResourceName);

			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);
		}

		DiffuseTextureMaterial(const NamedVarList& varList_) : Material(varList_), textureResourceName(StringID::None), textureInfo(nullptr){
			DiffuseTextureMaterial::Deserialize(varList_);
		}

		virtual ~DiffuseTextureMaterial() override{
			InvalidateTextureInfo();
		}

		virtual void Bind() override{
			if(shader == nullptr){
				LoadShader();
			}

			if(textureInfo == nullptr){
				CreateTextureInfo();
			}

			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);

			if(shader != nullptr){
				shader->Bind();
			}

			if(textureInfo != nullptr){
				textureInfo->Bind();
			}
		}

		virtual void Unbind() override{
			GADGET_BASIC_ASSERT(shader != nullptr);
			GADGET_BASIC_ASSERT(textureInfo != nullptr);

			if(textureInfo != nullptr){
				textureInfo->Unbind();
			}

			if(shader != nullptr){
				shader->Unbind();
			}
		}

		virtual StringID TextureResourceName() const{ return textureResourceName; }
		virtual bool HasLighting() const override{ return true; }

		virtual StringID Type() const override{ return type; }

		virtual void Serialize(NamedVarList& varList_) const override{
			Material::Serialize(varList_);
			varList_.Add(SID("TextureName"), textureResourceName);
		}

		void InvalidateTextureInfo(){
			delete textureInfo;
			textureInfo = nullptr;
		}

		virtual void InvalidateAllAPIInfos() override{
			Material::InvalidateAllAPIInfos();
			InvalidateTextureInfo();
		}

	protected:
		virtual void Deserialize(const NamedVarList& varList_) override{
			GADGET_BASIC_ASSERT(DiffuseTextureMaterial::Type() == varList_.GetValue(SID("MaterialType"), StringID::None).ToStr());
			textureResourceName = varList_.GetValue(SID("TextureName"), StringID::None).ToStr();
			GADGET_BASIC_ASSERT(textureResourceName != StringID::None);
		}

		void CreateTextureInfo(){
			InvalidateTextureInfo();
			Texture* tex = App::GetResourceManager().LoadResource<Texture>(textureResourceName);
			GADGET_ASSERT(tex != nullptr, "Could not load texture [" + textureResourceName.GetString() + "]!");

			if(tex != nullptr){
				textureInfo = App::GetRenderer().GenerateAPITextureInfo(*tex);
				GADGET_ASSERT(textureInfo != nullptr, "Could not generate texture info!");
			}

			App::GetResourceManager().UnloadResource(textureResourceName);
		}

	private:
		StringID textureResourceName;
		TextureInfo* textureInfo;
	};
}

#endif //!GADGET_DIFFUSE_TEXTURE_MATERIAL_H