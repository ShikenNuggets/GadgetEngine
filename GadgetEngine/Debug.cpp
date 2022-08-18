#include "Debug.h"

#include <iostream>

#include "Core/FileSystem.h"

using namespace Gadget;

Debug::LogType Debug::logLevel = Debug::Verbose;
std::set<StringID> Debug::logChannelFilter = std::set<StringID>();

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
		finalMessage += GetFileNameFromPath(fileName_);
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

std::string Debug::GetFileNameFromPath(const std::string& path_){
	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = path_.rfind(sep, path_.length());
	if(i != std::string::npos){
		return(path_.substr(i + 1, path_.length() - i));
	}
	return("");
}