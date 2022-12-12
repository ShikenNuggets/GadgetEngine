#ifndef GADGET_CONFIG_H
#define GADGET_CONFIG_H

#include <map>

#include "Events/Event.h"
#include "Utils/StringID.h"

namespace Gadget{
	class Config{
	public:
		static Config* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		float GetOptionFloat(StringID key_) const;
		bool GetOptionsBool(StringID key_) const;
		StringID GetOptionsString(StringID key_) const;

		void SetOption(StringID key_, int value_);
		void SetOption(StringID key_, float value_);
		void SetOption(StringID key_, bool value_);
		void SetOption(StringID key_, StringID value_);

		static void OnEvent(const Event& e_);

		//Config Keys
		static const StringID LanguageKey;
		static const StringID FullscreenKey;
		static const StringID WidthFullscreenKey;
		static const StringID HeightFullscreenKey;
		static const StringID WidthWindowedKey;
		static const StringID HeightWindowedKey;
		static const StringID LastWindowX;
		static const StringID LastWindowY;

	private:
		static Config* instance;
		static constexpr const char* engineConfigFile = "UserEngine.ini";

		Config();
		~Config();

		void SetDefaultEngineConfigs();

		std::map<StringID, float> floatOptions;
		std::map<StringID, bool> boolOptions;
		std::map<StringID, StringID> stringOptions;
	};
}

#endif //!GADGET_CONFIG_H