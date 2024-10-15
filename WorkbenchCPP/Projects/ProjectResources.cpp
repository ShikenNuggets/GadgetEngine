#include "ProjectResources.h"

using namespace Gadget;
using namespace Workbench;

ProjectResources::ProjectResources(const Project& project_) : resources(), project(project_){
	Load();
	const ErrorCode err = Save();
	if(err != ErrorCode::OK){
		GADGET_LOG_ERROR(SID("WBN"), "Could not save resources.json for the current project. Data may be lost!");
	}
}

void ProjectResources::AddResource(const StringID& name_){
	if(!resources.Contains(name_)){
		resources.Add(name_, NamedVarList(name_));
	}
}

void ProjectResources::RenameResource(const StringID& oldName_, const StringID& newName_){
	resources.Add(newName_, resources[oldName_]);
	resources.RemoveAt(oldName_);
}

void ProjectResources::SetVar(const StringID& name_, const StringID& varName_, const Var& value_){
	if(!resources.Contains(name_)){
		AddResource(name_);
	}

	resources[name_].SetValue(varName_, value_);
}

//TODO - This function needs *A LOT* more validation
void ProjectResources::Load(){
	if(!FileSystem::FileExists(project.GetPath() + ResourceManager::gResourcesJsonFile)){
		return;
	}

	const StaticArray<StringID, 7> expectedKeys{
		SID("path"),	SID("resource_type"),
		SID("path1"),	SID("path2"),
		SID("path3"),	SID("path4"),
		SID("path5"),
	};

	auto json = FileSystem::ReadPlainTextJSONFile(project.GetPath() + ResourceManager::gResourcesJsonFile);
	for(const auto& element : json){
		const std::string key = element.at(0).at("name");
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

ErrorCode ProjectResources::Save(){
	nlohmann::json j;

	for(const auto& r : resources){
		nlohmann::json element;
		element[0]["name"] = r.key;

		for(size_t i = 0; i < r.value.Size(); i++){
			element[1][r.value[i].Name().GetString()] = r.value[i].ToStr();
		}

		j.push_back(element);
	}

	return FileSystem::WriteJSONToPlainTextFile(project.GetPath() + ResourceManager::gResourcesJsonFile, j);
}