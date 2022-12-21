#ifndef GADGET_CONFIG_H
#define GADGET_CONFIG_H

#include <map>

#include "EngineVars.h"
#include "Events/Event.h"
#include "Utils/StringID.h"

namespace Gadget{
	class Config{
	public:
		static Config* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		Var GetOption(StringID key_) const;
		double GetOptionFloat(StringID key_) const;
		bool GetOptionsBool(StringID key_) const;
		StringID GetOptionsString(StringID key_) const;

		void SetOption(StringID section_, StringID key_, int32_t value_);
		void SetOption(StringID section_, StringID key_, int64_t value_);
		void SetOption(StringID section_, StringID key_, uint32_t value_);
		void SetOption(StringID section_, StringID key_, uint64_t value_);
		void SetOption(StringID section_, StringID key_, double value_);
		void SetOption(StringID section_, StringID key_, bool value_);
		void SetOption(StringID section_, StringID key_, StringID value_);

		void SaveConfigs();
		void ResetAllOptionsToDefault();

		static void OnEvent(const Event& e_);

	private:
		static Config* instance;
		static constexpr const char* engineConfigFileName = "UserEngine.ini";

		Config();
		~Config();

		std::string engineConfigPath;
		EngineVars vars;

		std::string CreateEngineConfigPath();
	};
}

#endif //!GADGET_CONFIG_H