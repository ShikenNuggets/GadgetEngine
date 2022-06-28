#include "Debug.h"

#include <iostream>

using namespace Gadget;

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
		case LogType::Critical:
			finalMessage = "CRITICAL ERROR: ";
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