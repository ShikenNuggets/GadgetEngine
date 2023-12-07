#include "FileSystem.h"

#include <filesystem>
#include <fstream>

#include "Debug.h"
#include "Utils/Utils.h"

#ifdef GADGET_PLATFORM_WIN32
#include "Platform/Windows/Win32_Utils.h"
#endif //GADGET_PLATFORM_WIN32

using namespace Gadget;

bool FileSystem::FileExists(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

	std::fstream filestream;
	filestream.open(filePath_, std::ios::in);
	return filestream.is_open();
}

bool FileSystem::DirExists(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());

	return std::filesystem::is_directory(path_);
}

std::vector<std::string> FileSystem::ReadFile(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

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

	fileContents.shrink_to_fit();

	filestream.close();
	return fileContents;
}

std::string FileSystem::ReadFileToString(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

	std::string result;
	std::vector<std::string> fileContents = ReadFile(filePath_);
	result.reserve(fileContents.size() * 16);
	for(std::string s : fileContents){
		result += s + "\n";
	}

	result.shrink_to_fit();
	return result;
}

std::vector<uint8_t> FileSystem::ReadBinaryFile(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

	std::ifstream input(filePath_, std::ios::binary);
	if(!input.is_open()){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for reading!", Debug::Error, __FILE__, __LINE__);
		return std::vector<uint8_t>();
	}

	return std::vector<uint8_t>(std::istreambuf_iterator<char>(input), {}); //TODO - This is not particularly efficient
}

nlohmann::json FileSystem::ReadPlainTextJSONFile(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

	if(!FileExists(filePath_)){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for reading!", Debug::Error, __FILE__, __LINE__);
		return nlohmann::json(nullptr);
	}

	return nlohmann::json::parse(ReadFileToString(filePath_));
}

nlohmann::json FileSystem::ReadBinaryJSONFile(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());

	if(!FileExists(filePath_)){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for reading!", Debug::Error, __FILE__, __LINE__);
		return nlohmann::json(nullptr);
	}

	return nlohmann::json::from_bson(ReadBinaryFile(filePath_)); //We're gonna use BSON https://bsonspec.org/
}

bool FileSystem::WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	//TODO - Check if file path is valid
	GADGET_BASIC_ASSERT(!content_.empty());
	GADGET_BASIC_ASSERT(type_ < WriteType::WriteType_MAX);

	if(!FileExists(filePath_)){
		CreateFile(filePath_);
	}

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
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}

	if(!filestream.is_open()){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return false;
	}

	filestream << content_;
	filestream.close();

	if(!filestream){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return false;
	}

	return true;
}

bool FileSystem::WriteToBinaryFile(const std::string& filePath_, const std::vector<uint8_t>& data_, WriteType type_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	//TODO - Check if file path is valid
	GADGET_BASIC_ASSERT(!data_.empty());
	GADGET_BASIC_ASSERT(type_ < WriteType::WriteType_MAX);

	if(!FileExists(filePath_)){
		CreateFile(filePath_);
	}

	std::ofstream outfile;
	switch(type_)
	{
		case WriteType::Append:
			Debug::Log("Appending to binary files is not recommended.", Debug::Warning, __FILE__, __LINE__);
			outfile.open(filePath_, std::ios::out | std::ios_base::app | std::ios::binary);
			break;
		case WriteType::Clear:
			outfile.open(filePath_, std::ios::out | std::ios_base::trunc | std::ios::binary);
			break;
		case WriteType::Overwrite:
			outfile.open(filePath_, std::ios::out | std::ios::binary);
			break;
		default:
			GADGET_ASSERT_NOT_IMPLEMENTED;
			break;
	}

	if(!outfile.is_open()){
		Debug::Log(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return false;
	}
	
	outfile.write(reinterpret_cast<const char*>(&data_[0]), data_.size());
	outfile.close();
	
	if(!outfile){
		Debug::Log(SID("FILESYSTEM"), "An error occurred while writing to file " + filePath_ + "!", Debug::Error, __FILE__, __LINE__);
		return false;
	}

	return true;
}

bool FileSystem::WriteJSONToPlainTextFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_){
	return FileSystem::WriteToFile(filePath_, json_.dump(4), type_);
}

bool FileSystem::WriteJSONToBinaryFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_){
	return WriteToBinaryFile(filePath_, nlohmann::json::to_bson(json_), type_);
}

std::string FileSystem::GetFileNameFromPath(const std::string& path_){
	size_t i = path_.rfind(PathSeparator, path_.length());
	if(i != std::string::npos){
		return(path_.substr(i + 1, path_.length() - i));
	}

	return "";
}

std::string FileSystem::GetPersistentDataDir(){
	#ifdef GADGET_PLATFORM_WIN32
		return Win32_Utils::GetUserDocumentsPath();
	#else
		static_assert(false, "Unhandled platform in FileSystem::GetDocumentsPath!");
	#endif //!GADGET_PLATFORM_WIN32
}

bool FileSystem::CreateFile(const std::string& file_){
	GADGET_BASIC_ASSERT(!file_.empty());
	//TODO - Check if file name is valid

	std::string dir = RemoveFileNameFromPath(file_);
	if(Utils::ContainsChar(dir, PathSeparator) && !DirExists(dir)){
		CreateDir(dir);
	}

	std::fstream filestream;
	filestream.open(file_, std::ios::out | std::ios_base::trunc);
	if(!filestream.is_open()){
		return false;
	}

	filestream.flush();
	filestream.close();
	return true;
}

bool FileSystem::CreateDir(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());
	//TODO - Check if path is valid

	if(DirExists(path_)){
		return false;
	}

	return std::filesystem::create_directory(path_);
}

std::string FileSystem::RemoveFileNameFromPath(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());
	return path_.substr(0, path_.find_last_of(PathSeparator));
}