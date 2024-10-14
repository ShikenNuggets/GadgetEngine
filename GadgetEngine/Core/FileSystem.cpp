#include "FileSystem.h"

#include <filesystem>
#include <fstream>

#include "Debug.h"
#include "Utils/Utils.h"

#ifdef GADGET_PLATFORM_WIN32
#include "Platform/Windows/Win32_Utils.h"
#endif //GADGET_PLATFORM_WIN32

using namespace Gadget;

std::string FileSystem::WorkingDir(){ return std::filesystem::current_path().string(); }

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

bool FileSystem::IsFileEmpty(const std::string& filePath_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	GADGET_BASIC_ASSERT(filePath_.size() == std::strlen(filePath_.c_str()));
	GADGET_BASIC_ASSERT(filePath_.size() == Utils::StrLen(filePath_.c_str()));
	GADGET_BASIC_ASSERT(FileExists(filePath_)); //You can call this function with a file path that doesn't exist, but the result may surprise you

	if(!FileExists(filePath_)){
		return false;
	}

	std::error_code err;
	const bool result = std::filesystem::is_empty(filePath_, err);
	if(err){
		GADGET_LOG_ERROR(SID("FILESYSTEM"), err.message());
	}

	return result;
}

bool FileSystem::IsDirEmpty(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());
	GADGET_BASIC_ASSERT(path_.size() == std::strlen(path_.c_str()));
	GADGET_BASIC_ASSERT(path_.size() == Utils::StrLen(path_.c_str()));
	GADGET_BASIC_ASSERT(DirExists(path_)); //You can call this function with a directory that doesn't exist, but the result may surprise you
	
	if(!DirExists(path_)){
		return false;
	}

	std::error_code err;
	const bool result = std::filesystem::is_empty(path_, err);
	if(err){
		GADGET_LOG_ERROR(SID("FILESYSTEM"), err.message());
	}

	return result;
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
	const std::vector<std::string> fileContents = ReadFile(filePath_);

	constexpr size_t averageCharsPerLine = 16; //Assumption - feel free to tweak this to see if it improves performance
	result.reserve(fileContents.size() * averageCharsPerLine);

	for(const std::string& s : fileContents){
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

ErrorCode FileSystem::WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	//TODO - Check if file path is valid
	GADGET_BASIC_ASSERT(!content_.empty());
	GADGET_BASIC_ASSERT(type_ < WriteType::WriteType_MAX);

	if(!FileExists(filePath_)){
		auto err = CreateFile(filePath_);
		if(err != ErrorCode::OK){
			return err;
		}
	}

	std::fstream filestream;

	switch(type_){
		case FileSystem::WriteType::Append:
			filestream.open(filePath_, std::fstream::out | std::fstream::app);
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
		Debug::LogToConsoleOnly(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::FileIO;
	}

	filestream << content_;
	filestream.close();

	if(!filestream){
		Debug::LogToConsoleOnly(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::FileIO;
	}

	return ErrorCode::OK;
}

ErrorCode FileSystem::WriteToBinaryFile(const std::string& filePath_, const std::vector<uint8_t>& data_, WriteType type_){
	GADGET_BASIC_ASSERT(!filePath_.empty());
	//TODO - Check if file path is valid
	GADGET_BASIC_ASSERT(!data_.empty());
	GADGET_BASIC_ASSERT(type_ < WriteType::WriteType_MAX);

	if(!FileExists(filePath_)){
		auto err = CreateFile(filePath_);
		if(err != ErrorCode::OK){
			return err;
		}
	}

	std::ofstream outfile;
	switch(type_){
		case WriteType::Append:
			Debug::LogToConsoleOnly(SID("FILESYSTEM"), "Appending to binary files is not recommended.", Debug::Warning, __FILE__, __LINE__);
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
		Debug::LogToConsoleOnly(SID("FILESYSTEM"), "Could not open " + filePath_ + " for writing!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::FileIO;
	}

	const std::ostreambuf_iterator<char> osbi(outfile);
	std::ranges::copy(data_, osbi);
	outfile.close();
	
	if(!outfile){
		Debug::LogToConsoleOnly(SID("FILESYSTEM"), "An error occurred while writing to file " + filePath_ + "!", Debug::Error, __FILE__, __LINE__);
		return ErrorCode::FileIO;
	}

	return ErrorCode::OK;
}

ErrorCode FileSystem::WriteJSONToPlainTextFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_){
	return FileSystem::WriteToFile(filePath_, json_.dump(4), type_);
}

ErrorCode FileSystem::WriteJSONToBinaryFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_){
	return WriteToBinaryFile(filePath_, nlohmann::json::to_bson(json_), type_);
}

std::string FileSystem::GetPersistentDataDir(){
	#ifdef GADGET_PLATFORM_WIN32
		return Win32_Utils::GetUserDocumentsPath();
	#else
		static_assert(false, "Unhandled platform in FileSystem::GetDocumentsPath!");
	#endif //!GADGET_PLATFORM_WIN32
}

ErrorCode FileSystem::CreateFile(const std::string& file_){
	GADGET_BASIC_ASSERT(!file_.empty());
	//TODO - Check if file name is valid

	const std::string dir = RemoveFileNameFromPath(file_);
	if(file_.find(PathSeparator) != std::string::npos && !DirExists(dir)){
		auto err = CreateDir(dir);
		if(err != ErrorCode::OK){
			return err;
		}
	}

	std::fstream filestream;
	filestream.open(file_, std::ios::out | std::ios_base::trunc);
	if(!filestream.is_open()){
		GADGET_LOG_WARNING(SID("FILESYSTEM"), "Could not create file. std error: " + std::string(std::strerror(errno)));
		return ErrorCode::FileIO;
	}

	filestream.flush();
	filestream.close();
	return ErrorCode::OK;
}

ErrorCode FileSystem::CreateDir(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());
	//TODO - Check if path is valid

	if(DirExists(path_)){
		return ErrorCode::OK;
	}

	return std::filesystem::create_directory(path_) ? ErrorCode::OK : ErrorCode::FileIO;
}

bool FileSystem::IsLastWriteTimeNewer(const std::string& basePath_, const std::string& comparePath_){
	GADGET_BASIC_ASSERT(!basePath_.empty());
	GADGET_BASIC_ASSERT(!comparePath_.empty());
	GADGET_BASIC_ASSERT(FileExists(basePath_));
	GADGET_BASIC_ASSERT(FileExists(comparePath_));

	if(!FileExists(basePath_) || !FileExists(comparePath_)){
		return false;
	}

	auto path1WriteTime = std::filesystem::last_write_time(basePath_);
	auto path2WriteTime = std::filesystem::last_write_time(comparePath_);

	return path2WriteTime < path1WriteTime;
}

std::string FileSystem::RemoveFileNameFromPath(const std::string& path_){
	GADGET_BASIC_ASSERT(!path_.empty());
	return path_.substr(0, path_.find_last_of(PathSeparator));
}