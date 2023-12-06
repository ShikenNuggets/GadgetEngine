#include "Debug.h"

#include <iostream>

#ifdef GADGET_PLATFORM_WIN32
#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL_messagebox.h>
#pragma warning(default : 26819)
#endif //GADGET_PLATFORM_WIN32

#include "App.h"
#include "Core/FileSystem.h"
#include "Utils/Utils.h"

using namespace Gadget;

std::string Debug::logFilePath = logFileName;
bool Debug::isInitialized = false;
Debug::LogType Debug::logLevel = Debug::Verbose;
std::set<StringID> Debug::logChannelFilter = std::set<StringID>();
std::queue<std::string> Debug::queuedLogsForFileWrite;

void Debug::Init(){
#ifdef GADGET_DEBUG
	constexpr auto writeType = FileSystem::WriteType::Append; //Keeping all session logs is ideal for debugging
	logFilePath = logFileName;
#else
	constexpr auto writeType = FileSystem::WriteType::Overwrite; //Deleting logs from old sesions is ideal for end users to avoid bloat
	logFilePath = FileSystem::GetPersistentDataDir() + FileSystem::PathSeparator + App::GetGameName() + FileSystem::PathSeparator + logFileName;
#endif //GADGET_DEBUG

	FileSystem::WriteToFile(logFilePath, "-------------------------\n" + Utils::GetCurrentDateAndTimeString() + " GMT\n", writeType);

	isInitialized = true;
	WriteQueuedLogs();
}

void Debug::Log(const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_){
	GADGET_BASIC_ASSERT(!message_.empty());
	GADGET_BASIC_ASSERT(type_ < LogType_MAX);

	if(type_ < logLevel){
		return; //Ignore log messages below the current log level
	}

	std::string finalMessage;
	switch(type_){
		case LogType::Verbose:
			finalMessage = "LOG: ";
			break;
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

	std::cout << finalMessage << std::endl;
	QueueLogForFileWrite(finalMessage + "\n");
}

void Debug::Log(StringID channel_, const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_){
	GADGET_BASIC_ASSERT(channel_ != StringID::None);
	GADGET_BASIC_ASSERT(!message_.empty());

	if(logChannelFilter.empty() || logChannelFilter.find(channel_) != logChannelFilter.end()){
		Debug::Log("[" + channel_.GetString() + "] " + message_, type_, fileName_, lineNumber_); //Only print if there is no filter set or if this channel is in the filter list
	}
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
	int status = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title_.c_str(), (message_ + "         \n         ").c_str(), nullptr);
	if(status != 0){
		Debug::Log(std::string("MessageBox couild not be shown. SDL Error: ") + SDL_GetError(), Debug::Error, __FILENAME__, __LINE__);
	}
	#else
	static_assert(false, "Unhandled platform in Debug::PopupErrorMessage!");
	#endif //GADGET_PLATFORM_WIN32
}

void Debug::ThrowFatalError(StringID channel_, const std::string& message_, const std::string& file_, int line_){
	GADGET_BASIC_ASSERT(channel_ != StringID::None);
	GADGET_BASIC_ASSERT(!message_.empty());

	Debug::Log(channel_, message_, FatalError, file_, line_);
	PopupErrorMessage("Fatal Error! [" + channel_.GetString() + "]", message_ + "\n\n" + FileSystem::GetFileNameFromPath(file_) + ":" + std::to_string(line_));
	throw std::runtime_error(message_ + "\n\n" + FileSystem::GetFileNameFromPath(file_) + ":" + std::to_string(line_));
}

void Debug::QueueLogForFileWrite(const std::string& message_){
	GADGET_BASIC_ASSERT(!message_.empty());

	queuedLogsForFileWrite.push(message_);
	WriteQueuedLogs();
}

void Debug::WriteQueuedLogs(){
	while(isInitialized && !queuedLogsForFileWrite.empty()){
		FileSystem::WriteToFile(logFilePath, queuedLogsForFileWrite.front());
		queuedLogsForFileWrite.pop();
	}
}