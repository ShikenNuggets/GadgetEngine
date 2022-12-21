#include "Debug.h"

#include <iostream>

#ifdef GADGET_PLATFORM_WIN32
#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL_messagebox.h>
#pragma warning(default : 26819)
#endif //GADGET_PLATFORM_WIN32

#include "Core/FileSystem.h"
#include "Utils/Utils.h"

using namespace Gadget;

Debug::LogType Debug::logLevel = Debug::Verbose;
std::set<StringID> Debug::logChannelFilter = std::set<StringID>();

void Debug::Init(){
	FileSystem::WriteToFile("log.txt", "-------------------------\n" + Utils::GetCurrentDateAndTimeString() + "GMT\n");
}

void Debug::Log(const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_){
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
	FileSystem::WriteToFile("log.txt", finalMessage + "\n");
}

void Debug::Log(StringID channel_, const std::string& message_, LogType type_, const std::string& fileName_, int lineNumber_){
	if(logChannelFilter.empty() || logChannelFilter.find(channel_) != logChannelFilter.end()){
		Debug::Log("[" + channel_.GetString() + "] " + message_, type_, fileName_, lineNumber_); //Only print if there is no filter set or if this channel is in the filter list
	}
}

Debug::LogType Debug::GetLogVerbosity(){ return logLevel; }

void Debug::SetLogVerbosity(Debug::LogType type_){ logLevel = type_; }

void Debug::AddFilter(StringID id_){
	logChannelFilter.insert(id_);
}

void Debug::ResetFilter(){
	logChannelFilter.clear();
}

void Debug::PopupErrorMessage(const std::string& title_, const std::string& message_){
	Debug::Log(SID("FATAL"), message_, FatalError);

	#ifdef GADGET_PLATFORM_WIN32
	//Extra spacing at the end to prevent text from getting cut off
	int status = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title_.c_str(), (message_ + "         \n         ").c_str(), nullptr);
	if(status != 0){
		Debug::Log(std::string("MessageBox couild not be shown. SDL Error: ") + SDL_GetError(), Debug::Error, __FILENAME__, __LINE__);
	}
	#else
	static_assert(false, "Unhandled platform in Debug::PopupErrorMessage!);
	#endif //GADGET_PLATFORM_WIN32
}