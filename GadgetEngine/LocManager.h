#ifndef GADGET_LOC_MANAGER_H
#define GADGET_LOC_MANAGER_H

#include <vector>

#include "Utils/StringID.h"

namespace Gadget{
	class LocManager{
	public:
		static LocManager* GetInstance();

		#ifdef GADGET_DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //GADGET_DEBUG

		StringID GetCurrentLanguage() const;

		void AddLanguage(StringID language_);
		void SetCurrentLanguage(StringID language_);

	private:
		static LocManager* instance;

		LocManager();
		~LocManager();

		//Bump this up to a uint16_t if you need to support more than 256 languages
		//There's ~7k living languages, but I've never seen a game officially support more than like 25
		uint8_t currentLanguage;

		std::vector<StringID> languages;
	};
}

#endif //!GADGET_LOC_MANAGER_H