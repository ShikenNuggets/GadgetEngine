#ifndef GADGET_DEBUG_H
#define GADGET_DEBUG_H

#include <set>
#include <string>

#include "Utils/StringID.h"

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

	private:
		static LogType logLevel;
		static std::set<StringID> logChannelFilter;

		static std::string GetFileNameFromPath(const std::string& path_); //Helper function, TODO - Move this to a filesystem class or something
	};
}

#endif //!GADGET_DEBUG_H