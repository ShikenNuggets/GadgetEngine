#ifndef GADGET_RESOURCE_CONTAINER_SERIALIZER_H
#define GADGET_RESOURCE_CONTAINER_SERIALIZER_H

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 26800)	//Use of a moved object
#pragma warning(disable : 26819)	//Unnanotated fallthrough in switch statement
#include <nlohmann/json.hpp>
#pragma warning(default : 26495)
#pragma warning(default : 26800)
#pragma warning(default : 26819)

#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Text/FreetypeFont.h"
#include "Graphics/OpenGL/GL_Shader.h"
#include "Resource/Resource.h"

namespace Gadget{
	inline void to_json(nlohmann::json& j_, const ResourceContainer* value_){
		GADGET_ASSERT(value_ != nullptr, "Attempting to serialize a nullptr!");

		auto glShader = dynamic_cast<const GL_ShaderResourceContainer*>(value_);
		if(glShader != nullptr){
			j_ = nlohmann::json{{"resource_type", GL_Shader::typeName }, {"path1", glShader->Path()}, {"path2", glShader->FragPath()}};
		}else{
			j_ = nlohmann::json{{"resource_type", value_->Type()}, {"path", value_->Path()}};
		}
	}

	//TODO - I feel like there's a better way to approach this
	inline void from_json(nlohmann::json& j_, ResourceContainer*& value_){
		std::string typeStr = j_.at("resource_type");
		StringID typeID = StringID::ProcessString(typeStr);

		if(typeID == SID("Mesh")){
			value_ = new MeshResourceContainer(j_.at("path"));
		}else if(typeID == SID("GL_Shader")){
			value_ = new GL_ShaderResourceContainer(j_.at("path1"), j_.at("path2"));
		}else if(typeID == SID("Font")){
			value_ = new FontResourceContainer(j_.at("path"));
		}else if(typeID == SID("FreetypeFont")){
			value_ = new FreetypeFontResourceContainer(j_.at("path"));
		}else if(typeID == SID("Texture")){
			value_ = new TextureResourceContainer(j_.at("path"));
		}else{
			GADGET_ASSERT_NOT_IMPLEMENTED;
		}
	}
}

#endif //!GADGET_RESOURCE_CONTAINER_SERIALIZER_H