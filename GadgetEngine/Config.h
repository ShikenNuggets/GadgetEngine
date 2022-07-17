#ifndef GADGET_CONFIG_H
#define GADGET_CONFIG_H

#include <map>

#include "Utils/StringID.h"

namespace Gadget{
	class Config{
	public:
		static Config* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		float GetOptionFloat(StringID key_) const;
		StringID GetOptionsString(StringID key_) const;

	private:
		static Config* instance;

		Config();
		~Config();

		std::map<StringID, float> floatOptions;
		std::map<StringID, StringID> stringOptions;
	};
}

#endif //!GADGET_CONFIG_H