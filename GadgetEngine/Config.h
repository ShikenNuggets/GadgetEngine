#ifndef GADGET_CONFIG_H
#define GADGET_CONFIG_H

#include <map>

#include "EngineVars.h"
#include "Events/Event.h"
#include "Utils/StringID.h"

namespace Gadget{
	//Keeps track of all of the engine's configurable options
	//If something about the engine should be customizable by a game developer or the end-user, you should get/set it through here
	class Config{
	public:
		Config();
		~Config();

		Var GetOption(StringID key_) const;
		double GetOptionFloat(StringID key_) const;
		bool GetOptionBool(StringID key_) const;
		StringID GetOptionString(StringID key_) const;

		void SetOption(StringID section_, StringID key_, int32_t value_);
		void SetOption(StringID section_, StringID key_, int64_t value_);
		void SetOption(StringID section_, StringID key_, uint32_t value_);
		void SetOption(StringID section_, StringID key_, uint64_t value_);
		void SetOption(StringID section_, StringID key_, double value_);
		void SetOption(StringID section_, StringID key_, bool value_);
		void SetOption(StringID section_, StringID key_, StringID value_);

		void SaveConfigs() const;

		void ResetOptionToDefault(StringID option_);
		void ResetSectionToDefault(StringID section_);
		void ResetAllOptionsToDefault(); //This will reset ALL EngineVars values in ALL subcategories

		static void OnEvent(const Event& e_);

	private:
		static constexpr const char* engineConfigFileName = "UserEngine.ini";

		std::string engineConfigPath;
		EngineVars vars;

		std::string CreateEngineConfigPath();
	};
}

#endif //!GADGET_CONFIG_H