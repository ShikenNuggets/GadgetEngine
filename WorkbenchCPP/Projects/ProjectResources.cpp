#include "ProjectResources.h"

using namespace Gadget;
using namespace Workbench;

ProjectResources::ProjectResources(const Project& project_) : resources(), project(project_){
	Load();
}

void ProjectResources::AddResource(const StringID& name_){
	if(!resources.Contains(name_)){
		resources.Add(name_, NamedVarList(name_));
	}
}

void ProjectResources::SetVar(const StringID& name_, const StringID& varName_, const Var& value_){
	if(!resources.Contains(name_)){
		AddResource(name_);
	}

	if(resources[name_].HasValue(varName_)){
		resources[name_].SetValue(varName_, value_);
	}
}

void ProjectResources::Load(){
	const StaticArray<StringID, 7> expectedKeys{
		SID("path"),	SID("resource_type"),
		SID("path1"),	SID("path2"),
		SID("path3"),	SID("path4"),
		SID("path5"),
	};

	auto json = FileSystem::ReadPlainTextJSONFile(project.GetPath() + ResourceManager::gResourcesJsonFile);
	for(const auto& element : json){
		const std::string key = element.at(0).at("string");
		StringID keyID = StringID::ProcessString(key);
		AddResource(keyID);

		auto& var = element.at(1);
		for(const auto& vkey : expectedKeys){
			if(var.contains(vkey.GetString())){
				SetVar(keyID, vkey, std::string(var.at(vkey.GetString())));
			}
		}
	}
}

ErrorCode ProjectResources::Save(){ return ErrorCode::OK; }