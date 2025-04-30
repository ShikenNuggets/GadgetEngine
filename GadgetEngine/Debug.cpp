#include "Debug.h"

#include <iostream>

#ifdef GADGET_PLATFORM_WIN32
//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26819) //Unnanotated fallthrough in switch statement
#include <SDL3/SDL_messagebox.h>
#pragma warning(default : 26819)
#endif //GADGET_PLATFORM_WIN32

#include "App.h"
#include "Core/FileSystem.h"
#include "Platform/PlatformUtils.h"
#include "Utils/Utils.h"

using namespace Gadget;

std::string Debug::logFilePath = logFileName;
bool Debug::isInitialized = false;
Debug::LogType Debug::logLevel = Debug::Verbose;
std::set<StringID> Debug::logChannelFilter = std::set<StringID>();

std::queue<std::string> Debug::queuedLogsForFileWrite;
std::mutex Debug::logQueueMutex;

void Debug::Init(){
	GADGET_BASIC_ASSERT(!isInitialized);
	if(isInitialized){
		std::cout << "Tried to initialize debug subsystem twice!\n";
		return;
	}

#ifdef GADGET_DEBUG
	constexpr auto writeType = FileSystem::WriteType::Append; //Keeping all session logs is ideal for debugging
	logFilePath = logFileName;
#else
	constexpr auto writeType = FileSystem::WriteType::Overwrite; //Deleting logs from old sesions is ideal for end users to avoid bloat
	logFilePath = FileSystem::GetPersistentDataDir() + FileSystem::PathSeparator + App::GetGameName() + FileSystem::PathSeparator + logFileName;
#endif //GADGET_DEBUG

	const std::string message = "-------------------------\n" + Utils::GetCurrentDateAndTimeString() + " GMT\n";
	auto err = FileSystem::WriteToFile(logFilePath, message, writeType);
	if(err != ErrorCode::OK){
		std::cout << "ERROR: Could not write initial log message to " << FileSystem::GetFileNameFromPath(logFilePath) << "! We'll try again later...\n";

		{
			const std::lock_guard lock{ logQueueMutex };
			queuedLogsForFileWrite.push(message);
		}
	}

	isInitialized = true;
	WriteQueuedLogs();
}

void Debug::Log(const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_, bool writeToLogFile_){
	GADGET_BASIC_ASSERT(!message_.empty());
	GADGET_BASIC_ASSERT(type_ < LogType_MAX);

	if(type_ < logLevel){
		return; //Ignore log messages below the current log level
	}

	std::string finalMessage;
	switch(type_){
		case LogType::Verbose: [[fallthrough]];
		case LogType::Info:
			finalMessage = "LOG: ";
			break;
		case LogType::Warning:
			finalMessage = "WARNING: ";
			break;
		case LogType::Error:
			finalMessage = "ERROR: ";
			break;
		case LogType::FatalError:
			finalMessage = "FATAL ERROR: ";
			break;
		default:
			break;
	}

	finalMessage += message_;
	if(!fileName_.empty() && lineNumber_ > 0){
		finalMessage += " (";
		finalMessage += FileSystem::GetFileNameFromPath(fileName_);
		finalMessage += ", ";
		finalMessage += std::to_string(lineNumber_);
		finalMessage += ")";
	}

#ifdef GADGET_PLATFORM_WIN32
	switch(type_){
		case Verbose: [[fallthrough]];
		case Info:
			PlatformUtils::SetConsoleColorWhite();
			break;
		case Warning:
			PlatformUtils::SetConsoleColorYellow();
			break;
		case Error: [[fallthrough]];
		case FatalError:
			PlatformUtils::SetConsoleColorRed();
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}
#endif //GADGET_PLATFORM_WIN32

	std::cout << finalMessage << "\n";

#ifdef GADGET_PLATFORM_WIN32
	PlatformUtils::SetConsoleColorWhite();

	PlatformUtils::OutputToDebuggerConsole(finalMessage + "\n");
#endif //GADGET_PLATFORM_WIN32

	if(writeToLogFile_){
		QueueLogForFileWrite(finalMessage + "\n");
	}
}

void Debug::Log(StringID channel_, const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_, bool writeToLogFile_){
	GADGET_BASIC_ASSERT(channel_ != StringID::None);
	GADGET_BASIC_ASSERT(!message_.empty());
	GADGET_BASIC_ASSERT(type_ < LogType::LogType_MAX);

	if(logChannelFilter.empty() || logChannelFilter.find(channel_) != logChannelFilter.end()){
		Debug::Log("[" + channel_.GetString() + "] " + message_, type_, fileName_, lineNumber_, writeToLogFile_); //Only print if there is no filter set or if this channel is in the filter list
	}
}

void Debug::LogToConsoleOnly(StringID channel_, const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_){
	Debug::Log(channel_, message_, type_, fileName_, lineNumber_, false);
}

Debug::LogType Debug::GetLogVerbosity(){ return logLevel; }

void Debug::SetLogVerbosity(Debug::LogType type_){ logLevel = type_; }

void Debug::AddFilter(StringID id_){
	GADGET_BASIC_ASSERT(id_ != StringID::None);
	logChannelFilter.insert(id_);
}

void Debug::ResetFilter(){
	logChannelFilter.clear();
}

void Debug::PopupErrorMessage(const std::string& title_, const std::string& message_){
	GADGET_BASIC_ASSERT(!title_.empty());
	GADGET_BASIC_ASSERT(!message_.empty());

	Debug::Log(SID("FATAL"), message_, FatalError);

	#ifdef GADGET_PLATFORM_WIN32
	//Extra spacing at the end to prevent text from getting cut off
	const bool status = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title_.c_str(), (message_ + "         \n         ").c_str(), nullptr);
	if(!status){
		Debug::Log(std::string("MessageBox couild not be shown. SDL Error: ") + SDL_GetError(), Debug::Error, Gadget::FileSystem::GetFileNameFromPath(__FILE__), __LINE__);
	}
	#else
	static_assert(false, "Unhandled platform in Debug::PopupErrorMessage!");
	#endif //GADGET_PLATFORM_WIN32
}

void Debug::ThrowFatalError(StringID channel_, const std::string& message_, ErrorCode err_, const std::string& file_, int line_){
#ifndef GADGET_RELEASE
	//This is redundant, but it helps devs get the current callstack. You won't get if you wait for the throw to play out
	GADGET_ASSERT(false, "A fatal error was thrown from " + FileSystem::GetFileNameFromPath(file_) + "!");
#endif //!GADGET_RELEASE

	GADGET_BASIC_ASSERT(channel_ != StringID::None);
	GADGET_BASIC_ASSERT(!message_.empty());
	GADGET_BASIC_ASSERT(err_ > ErrorCode::OK && err_ < ErrorCode::ErrorCode_MAX);

	const std::string messageWithErr = message_ + std::string("\nError Code: ") + GetErrorCodeString(err_);
	const std::string finalMessage = messageWithErr + "\n\n" + FileSystem::GetFileNameFromPath(file_) + ":" + std::to_string(line_);

	Debug::Log(channel_, messageWithErr, FatalError, file_, line_);
	PopupErrorMessage("Fatal Error! [" + channel_.GetString() + "]", finalMessage);
	throw std::runtime_error(finalMessage);
}

void Debug::QueueLogForFileWrite(const std::string& message_){
	GADGET_BASIC_ASSERT(!message_.empty());

	{
		const std::lock_guard lock{ logQueueMutex };
		queuedLogsForFileWrite.push(message_);
	}

	WriteQueuedLogs();
}

void Debug::WriteQueuedLogs(){
	const std::lock_guard lock{ logQueueMutex };
	while(isInitialized && !queuedLogsForFileWrite.empty()){
		auto err = FileSystem::WriteToFile(logFilePath, queuedLogsForFileWrite.front());
		if(err != ErrorCode::OK){
			Debug::LogToConsoleOnly(SID("DEBUG"), "Could not write log messages to " + FileSystem::GetFileNameFromPath(logFilePath) + "! We'll try again later...", Debug::Error, __FILE__, __LINE__);
			break;
		}

		queuedLogsForFileWrite.pop();
	}
}