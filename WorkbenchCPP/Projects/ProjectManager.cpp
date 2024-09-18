#include "ProjectManager.h"

#include <Core/FileSystem.h>

using namespace Gadget::Workbench;

static constexpr const char* gRecentProjectsFilePath = "recent_projects.json";

ProjectManager::ProjectManager() : projects(){
	if(FileSystem::FileExists(gRecentProjectsFilePath)){
		auto json = FileSystem::ReadPlainTextJSONFile(gRecentProjectsFilePath);

		if(!json.is_null()){
			for(auto& element : json){
				const String name = static_cast<std::string>(element.at("name"));
				const String path = static_cast<std::string>(element.at("path"));

				projects.Emplace(name, path);
			}
		}
	}

	//TODO
	//ErrorCode err = FileSystem::WriteJSONToPlainTextFile(gRecentProjectsFilePath, {});
	//if(err != ErrorCode::OK){
	//	Debug::ThrowFatalError(SID("PROJECT"), "ProjectManager could not write recent projects file!", err, __FILE__, __LINE__);
	//}
}