#ifndef GADGET_DEBUG_H
#define GADGET_DEBUG_H

#include <mutex>
#include <queue>
#include <set>
#include <string>

//Be careful including Gadget headers here - A lot of the engine depends on this file
#include "GadgetEnums.h"
#include "Core/FileSystem.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"

//These macros are very important so make extra sure this is the only place they're being defined, and that they're only defined once
#if defined(__FILENAME__) || defined(GADGET_ASSERT) || defined(GADGET_BASIC_ASSERT) || defined(GADGET_ASSERT_NOT_IMPLEMENTED) || defined(GADGET_DEBUG_INT) || defined(GADGET_LOG) || defined(GADGET_LOG_WARNING) || defined(GADGET_LOG_ERROR)
	#error "Essential macros are being redefined"
#endif

#if defined GADGET_DEBUG && defined GADGET_PLATFORM_WIN32
	#define GADGET_ASSERT(expr, msg) { if(!(expr)){ Gadget::Debug::PopupErrorMessage(std::string("Assert Failed! ") + Gadget::FileSystem::GetFileNameFromPath(__FILE__) + std::string(" : ") + std::to_string(__LINE__), msg); __debugbreak(); } }
#endif //GADGET_DEBUG && GADGET_PLATFORM_WIN32

#if defined GADGET_RELEASE || !defined GADGET_PLATFORM_WIN32
	//We still want the poup messages but not the debug break
	//In an actual final production build you'll probably want to remove these entirely
	#define GADGET_ASSERT(expr, msg) { if(!(expr)){ Gadget::Debug::PopupErrorMessage(std::string("Assert Failed! ") + Gadget::FileSystem::GetFileNameFromPath(__FILE__) + std::string(" : ") + std::to_string(__LINE__), msg); } }
#endif //GADGET_RELEASE || !GADGET_PLATFORM_WIN32

#define GADGET_BASIC_ASSERT(expr) GADGET_ASSERT(expr, "Condition Failed: " #expr)
#define GADGET_ASSERT_NOT_IMPLEMENTED GADGET_ASSERT(false, "Case not implemented - Ask a dev!")
#define GADGET_ASSERT_UNREACHABLE GADGET_ASSERT(false, "Executing code that was expected to be unreachable - Ask a dev!");
#define GADGET_ASSERT_FILE_EXISTS(expr) GADGET_ASSERT(FileSystem::FileExists(expr), std::string("Condition Failed: FileSystem::FileExists(") + expr + ")");

#if defined GADGET_RELEASE || defined GADGET_FAST_ASSERTS
	#define GADGET_SLOW_ASSERT(expr)
	#define GADGET_SLOW_ASSERT_MSG(expr, msg)
#else
	#define GADGET_SLOW_ASSERT(expr) GADGET_BASIC_ASSERT(expr)
	#define GADGET_SLOW_ASSERT_MSG(expr, msg) GADGET_ASSERT(expr, msg)
#endif

#if defined GADGET_DEBUG
	#define GADGET_DEBUG_INT(var) int var
#else
	#define GADGET_DEBUG_INT(var) ;
#endif

namespace Gadget{
	class Debug{
	public:
		STATIC_CLASS(Debug);

		enum LogType : uint8_t{
			Verbose		= 0,
			Info		= 1,
			Warning		= 2,
			Error		= 3,
			FatalError	= 4,

			LogType_MAX //Put new values ABOVE this, nothing should be below
		};

		static void Init();

		static void Log(const std::string& message_, LogType type_ = LogType::Info, const std::string& fileName_ = "", int lineNumber_ = 0, bool writeToLogFile_ = true);
		static void Log(StringID channel_, const std::string& message_, LogType type_ = LogType::Info, const std::string& fileName_ = "", int lineNumber = 0, bool writeToLogFile_ = true);

		//For cases where writing to the log file could cause issues (e.g. *in* file write operations)
		//In most cases you should just use Log directly
		static void LogToConsoleOnly(StringID channel_, const std::string& message_, LogType type_ = LogType::Info, const std::string& fileName_ = "", int lineNumber = 0);

		static LogType GetLogVerbosity();
		static void SetLogVerbosity(LogType type_);

		static void AddFilter(StringID id_);
		static void ResetFilter();

		static void PopupErrorMessage(const std::string& title_, const std::string& message_);

		//ONLY USE THIS FOR *COMPLETELY UNRECOVERABLE* ERROR CASES - THIS *WILL* CRASH THE GAME
		[[noreturn]] static void ThrowFatalError(StringID channel_, const std::string& message_, ErrorCode err_, const std::string& file_, int line_);

	private:
		static constexpr const char* logFileName = "log.txt";
		static std::string logFilePath;
		static bool isInitialized;
		static LogType logLevel;
		static std::set<StringID> logChannelFilter;

		static std::queue<std::string> queuedLogsForFileWrite;
		static std::mutex logQueueMutex;

		static void QueueLogForFileWrite(const std::string& message_);
		static void WriteQueuedLogs();
	};
}

#ifndef GADGET_LOG
	#define GADGET_LOG(channel, message) Gadget::Debug::Log(channel, message, Gadget::Debug::Info, __FILE__, __LINE__)
#endif //!GADGET_LOG

#ifndef GADGET_LOG_WARNING
	#define GADGET_LOG_WARNING(channel, message) Gadget::Debug::Log(channel, message, Gadget::Debug::Warning, __FILE__, __LINE__)
#endif //!GADGET_LOG_WARNING

#ifndef GADGET_LOG_ERROR
	#define GADGET_LOG_ERROR(channel, message) Gadget::Debug::Log(channel, message, Gadget::Debug::Error, __FILE__, __LINE__)
#endif //!GADGET_LOG_ERROR


#endif //!GADGET_DEBUG_H