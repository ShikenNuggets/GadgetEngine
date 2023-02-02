#include "SkyboxComponent.h"

#include "App.h"

using namespace Gadget;

SkyboxComponent::SkyboxComponent(Scene* parent_, StringID skyboxName_, StringID shader_) : SceneComponent(parent_), shaderName(shader_), shader(nullptr), cubemapInfo(nullptr){
	shader = App::GetInstance().GetRenderer().GenerateAPIShader(shaderName);

	std::array<StringID, 6> textures = {
		StringID::ProcessString(skyboxName_.GetString() + "RightTexture"),
		StringID::ProcessString(skyboxName_.GetString() + "LeftTexture"),
		StringID::ProcessString(skyboxName_.GetString() + "TopTexture"),
		StringID::ProcessString(skyboxName_.GetString() + "BottomTexture"),
		StringID::ProcessString(skyboxName_.GetString() + "FrontTexture"),
		StringID::ProcessString(skyboxName_.GetString() + "BackTexture"),
	};

	Cubemap* cubemap = new Cubemap(textures[0], textures[1], textures[2], textures[3], textures[4], textures[5]);
	cubemapInfo = new GL_CubemapInfo(*cubemap);
	delete cubemap;
}

SkyboxComponent::~SkyboxComponent(){
	delete cubemapInfo;
	App::GetInstance().GetResourceManager().UnloadResource(shaderName);
}

void SkyboxComponent::Bind(const Matrix4& proj_, const Matrix4& view_){
	shader->Bind();
	shader->BindMatrix4(SID("projectionMatrix"), proj_);
	shader->BindMatrix4(SID("viewMatrix"), view_);
	cubemapInfo->Bind();
}

void SkyboxComponent::Unbind(){
	cubemapInfo->Unbind();
	shader->Unbind();
}