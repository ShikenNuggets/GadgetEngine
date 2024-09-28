#ifndef GADGET_WORKBENCH_PROJECTS_PROJECT_H
#define GADGET_WORKBENCH_PROJECTS_PROJECT_H

#include <chrono>
#include <string>

#include <nlohmann/json.hpp>

namespace Gadget::Workbench{
	class Project{
	public:
		Project() : name(), path(){}
		Project(std::string name_, std::string path_) : name(std::move(name_)), path(std::move(path_)){}

		const std::string& GetName() const{ return name; }
		const std::string& GetPath() const{ return path; }

		constexpr inline std::string FullProjectFilePath() const{ return path + name + gProjectExtension; }

	private:
		static constexpr inline const char* gProjectExtension = ".wbn";

		std::string name;
		std::string path;
	};
}

namespace Gadget{
	inline void to_json(nlohmann::json& j_, const Workbench::Project& p_){
		j_["name"] = p_.GetName();
		j_["path"] = p_.GetPath();
	}

	inline void from_json(const nlohmann::json& j_, Workbench::Project& p_){
		p_ = Workbench::Project(j_.at("name"), j_.at("path"));
	}
}

#endif //!GADGET_WORKBENCH_PROJECTS_PROJECT_H