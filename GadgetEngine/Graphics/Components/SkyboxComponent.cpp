#include "SkyboxComponent.h"

#include "App.h"

using namespace Gadget;

static constexpr size_t gNumSides = 6;

SkyboxComponent::SkyboxComponent(Scene* parent_, StringID skyboxName_, StringID shader_) : SceneComponent(parent_), shaderName(shader_), shader(nullptr), cubemapInfo(nullptr){
	GADGET_BASIC_ASSERT(parent_ != nullptr);
	GADGET_BASIC_ASSERT(skyboxName_ != StringID::None);
	GADGET_BASIC_ASSERT(shader_ != StringID::None);
	
	shader = App::GetRenderer().GenerateAPIShader(shaderName);

	std::array<StringID, gNumSides> textures = {
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

	GADGET_BASIC_ASSERT(shader != nullptr);
	GADGET_BASIC_ASSERT(cubemapInfo != nullptr);
}

SkyboxComponent::~SkyboxComponent(){
	delete cubemapInfo;
	App::GetResourceManager().UnloadResource(shaderName);
}

void SkyboxComponent::Bind(const Matrix4& proj_, const Matrix4& view_){
	GADGET_BASIC_ASSERT(shader != nullptr);
	GADGET_BASIC_ASSERT(cubemapInfo != nullptr);
	GADGET_BASIC_ASSERT(proj_.IsValid());
	GADGET_BASIC_ASSERT(view_.IsValid());

	shader->Bind();
	shader->BindMatrix4(SID("projectionMatrix"), proj_);
	shader->BindMatrix4(SID("viewMatrix"), view_);
	cubemapInfo->Bind();
}

void SkyboxComponent::Unbind(){
	GADGET_BASIC_ASSERT(shader != nullptr);
	GADGET_BASIC_ASSERT(cubemapInfo != nullptr);

	cubemapInfo->Unbind();
	shader->Unbind();
}