#include "FileSystem.h"

#include "Debug.h"

using namespace Gadget;

bool FileSystem::FileExists(const std::string& filePath_){
	std::fstream filestream;
	filestream.open(filePath_, std::ios::in);
	return filestream.is_open();
}

std::vector<std::string> FileSystem::ReadFile(const std::string& filePath_){
	std::vector<std::string> fileContents;

	std::fstream filestream;
	filestream.open(filePath_, std::ios::in);

	if(!filestream.is_open()){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for reading!", Debug::Error, __FILE__, __LINE__);
		return fileContents;
	}

	std::string line;
	while(std::getline(filestream, line)){
		fileContents.push_back(line);
	}

	filestream.close();
	return fileContents;
}

std::string FileSystem::ReadFileToString(const std::string& filePath_){
	std::string result;
	std::vector<std::string> fileContents = ReadFile(filePath_);
	for(std::string s : fileContents){
		result += s + "\n";
	}

	return result;
}

void FileSystem::WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_){
	std::fstream filestream;

	switch(type_){
		case FileSystem::WriteType::Append:
			filestream.open(filePath_, std::ios::out | std::ios_base::app);
			break;
		case FileSystem::WriteType::Clear:
			filestream.open(filePath_, std::ios::out | std::ios_base::trunc);
			break;
		case FileSystem::WriteType::Overwrite:
			filestream.open(filePath_, std::ios::out);
			break;
		default:
			break;
	}

	if(!filestream.is_open()){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return;
	}

	filestream << content_;
	filestream.close();
}