#ifndef GADGET_DEBUG_H
#define GADGET_DEBUG_H

#include <set>
#include <string>

#include "Utils/StringID.h"

#ifdef GADGET_PLATFORM_WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif


#if defined GADGET_DEBUG && defined GADGET_PLATFORM_WIN32
	#define GADGET_ASSERT(expr, msg) if(!(expr)){ Debug::PopupErrorMessage(std::string("Assert Failed! ") + __FILENAME__ + " : " + std::to_string(__LINE__), msg); __debugbreak(); }
	#define GADGET_BASIC_ASSERT(expr) GADGET_ASSERT(expr, "Condition Failed: " ## #expr)
#endif //GADGET_DEBUG && GADGET_PLATFORM_WIN32

#if defined GADGET_RELEASE || !defined GADGET_PLATFORM_WIN32
	//We still want the poup messages but not the debug break
	//In an actual final production build you'll probably want to remove these entirely
	#define GADGET_ASSERT(expr, msg) if(!(expr)){ Debug::PopupErrorMessage(std::string("Assert Failed! ") + __FILENAME__ + " : " + std::to_string(__LINE__), msg); }
	#define GADGET_BASIC_ASSERT(expr) GADGET_ASSERT(expr, "Condition Failed: " ## #expr)
#endif //GADGET_RELEASE || !GADGET_PLATFORM_WIN32

namespace Gadget{
	class Debug{
	public:
		enum LogType{
			Verbose		= 0,
			Info		= 1,
			Warning		= 2,
			Error		= 3,
			FatalError	= 4
		};

		static void Log(const std::string& message_, LogType type_ = LogType::Info, const std::string& fileName_ = "", int lineNumber = 0);
		static void Log(StringID channel_, const std::string& message_, LogType type_ = LogType::Info, const std::string& fileName_ = "", int lineNumber = 0);

		static LogType GetLogVerbosity();
		static void SetLogVerbosity(LogType type_);

		static void AddFilter(StringID id_);
		static void ResetFilter();

		static void PopupErrorMessage(const std::string& title_, const std::string& message_);

	private:
		static LogType logLevel;
		static std::set<StringID> logChannelFilter;
	};
}

#endif //!GADGET_DEBUG_H