#include "ResourceManager.h"

#include "App.h"
#include "Core/FileSystem.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/OpenGL/GL_Shader.h"
#include "Graphics/Text/FreetypeFont.h"
#include "Serializers/ResourceContainerSerializer.h"

using namespace Gadget;

ResourceManager::ResourceManager() : resources(), maxUnusedResourceMemory(64ULL * 1024 * 1024){
	if(!FileSystem::FileExists(gResourcesJsonFile)){
		Debug::ThrowFatalError(SID("RESOURCE"), "resources.json file does not exist!", ErrorCode::FileIO, __FILE__, __LINE__);
	}

	auto resJson = FileSystem::ReadPlainTextJSONFile(gResourcesJsonFile);
	//resources = resJson; //TODO - Ideally this would work, but I'm just getting template errors that are hard to diagnose

	if(resJson.is_null()){
		Debug::ThrowFatalError(SID("RESOURCE"), "An error occured while loading resources.json!", ErrorCode::FileIO, __FILE__, __LINE__);
	}

	//Parsing manually for now
	for(auto& element : resJson){
		const std::string key = element.at(0).at("name");
		ResourceContainer* ptr = nullptr;
		from_json(element.at(1), ptr);
		resources.emplace(StringID::ProcessString(key), ptr);
	}
}

ResourceManager::~ResourceManager(){
	ForceDeleteAllUnusedResources();

	for(auto& c : resources){
		if(c.second != nullptr){
			GADGET_ASSERT(c.second->GetReferenceCount() == 0, "ResourceManager being destroyed while Resource [" + c.first.GetString() + "] is still referenced!");
		}

		delete c.second;
		c.second = nullptr;
	}

	resources.clear();
}

void ResourceManager::UnloadResource(StringID name_){
	GADGET_BASIC_ASSERT(name_ != StringID::None);

	if(resources.find(name_) == resources.end() || resources[name_] == nullptr){
		Debug::Log(SID("RESOURCE"), "Attempted to unload invalid resource [" + name_.GetString() + "]", Debug::Warning, __FILE__, __LINE__);
	}

	resources[name_]->RemoveReference();
	if(resources[name_]->GetReferenceCount() == 0){
		resourcesToUnload.Add(name_);
	}
}

size_t ResourceManager::GetMemoryUsageOfAllResources() const{
	size_t resourceSize = 0;
	for(const auto& r : resources){
		GADGET_BASIC_ASSERT(r.second != nullptr);
		if(r.second == nullptr){
			continue;
		}

		resourceSize += r.second->ResourceSizeInBytes();
	}

	return resourceSize;
}

size_t ResourceManager::GetMemoryUsageOfUnusedResources() const{
	size_t resourceSize = 0;
	for(const auto& r : resources){
		GADGET_BASIC_ASSERT(r.second != nullptr);
		if(r.second == nullptr){
			continue;
		}

		if(r.second->GetReferenceCount() == 0){
			resourceSize += r.second->ResourceSizeInBytes();
		}
	}

	return resourceSize;
}

void ResourceManager::Cleanup(){
	if(GetMemoryUsageOfUnusedResources() < maxUnusedResourceMemory){
		return;
	}

	const StringID resourceName = resourcesToUnload.Peek();
	resourcesToUnload.Remove();
	GADGET_BASIC_ASSERT(resources.contains(resourceName));
	if(resources.contains(resourceName)){
		resources[resourceName]->DeleteIfUnused();
	}
}

void ResourceManager::ForceDeleteAllUnusedResources(){
	for(auto& c : resources){
		GADGET_BASIC_ASSERT(c.second != nullptr);
		if(c.second != nullptr){
			c.second->DeleteIfUnused();
		}
	}
}