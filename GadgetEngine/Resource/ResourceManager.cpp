#include "ResourceManager.h"

#include "Graphics/Texture.h"
#include "Graphics/OpenGL/GL_Shader.h"

using namespace Gadget;

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager* ResourceManager::GetInstance(){
	if(instance == nullptr){
		instance = new ResourceManager();
	}

	return instance;
}

#ifdef GADGET_DEBUG
void ResourceManager::DeleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = nullptr;
	}
}
#endif //GADGET_DEBUG

ResourceManager::ResourceManager(){
	//TODO - Load resource list from some kind of file. JSON?
	resources.insert(std::make_pair(SID("DefaultShader"), new GL_ShaderResourceContainer("Resources/defaultVert.glsl", "Resources/defaultFrag.glsl")));
	resources.insert(std::make_pair(SID("ScreenShader"), new GL_ShaderResourceContainer("Resources/screenVert.glsl", "Resources/screenFrag.glsl")));
	resources.insert(std::make_pair(SID("ScreenInvertShader"), new GL_ShaderResourceContainer("Resources/screenVert.glsl", "Resources/screenInversionEffectFrag.glsl")));
	resources.insert(std::make_pair(SID("ScreenGrayscaleShader"), new GL_ShaderResourceContainer("Resources/screenVert.glsl", "Resources/screenGrayscaleFrag.glsl")));
	resources.insert(std::make_pair(SID("SkyboxShader"), new GL_ShaderResourceContainer("Resources/skyboxVert.glsl", "Resources/skyboxFrag.glsl")));

	resources.insert(std::make_pair(SID("CubeTexture"), new TextureResourceContainer("Resources/wall.bmp")));

	resources.insert(std::make_pair(SID("SkyboxRightTexture"), new TextureResourceContainer("Resources/skybox/right.bmp")));
	resources.insert(std::make_pair(SID("SkyboxLeftTexture"), new TextureResourceContainer("Resources/skybox/left.bmp")));
	resources.insert(std::make_pair(SID("SkyboxTopTexture"), new TextureResourceContainer("Resources/skybox/top.bmp")));
	resources.insert(std::make_pair(SID("SkyboxBottomTexture"), new TextureResourceContainer("Resources/skybox/bottom.bmp")));
	resources.insert(std::make_pair(SID("SkyboxFrontTexture"), new TextureResourceContainer("Resources/skybox/front.bmp")));
	resources.insert(std::make_pair(SID("SkyboxBackTexture"), new TextureResourceContainer("Resources/skybox/back.bmp")));
}

ResourceManager::~ResourceManager(){
	for(auto& c : resources){
		delete c.second;
		c.second = nullptr;
	}

	resources.clear();
}

void ResourceManager::UnloadResource(StringID name_){
	if(resources.find(name_) == resources.end() || resources[name_] == nullptr){
		Debug::Log(SID("RESOURCE"), "Attempted to unload invalid resource [" + name_.GetString() + "]", Debug::Warning, __FILE__, __LINE__);
	}

	resources[name_]->UnloadResource();
}