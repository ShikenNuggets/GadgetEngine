#ifndef GADGET_FILE_SYSTEM_H
#define GADGET_FILE_SYSTEM_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace Gadget{
	class FileSystem{
	public:
		enum class WriteType{
			Append,
			Clear,
			Overwrite,
		};

	#ifdef GADGET_PLATFORM_WIN32
		static constexpr char PathSeparator = '\\';
	#else
		static constexpr char PathSeparator = '/';
	#endif //GADGET_PLATFORM_WIN32
		
		static bool FileExists(const std::string& filePath_);
		static bool DirExists(const std::string& path_);

		static std::vector<std::string> ReadFile(const std::string& filePath_);
		static std::string ReadFileToString(const std::string& filePath_);
		static std::vector<uint8_t> ReadBinaryFile(const std::string& filePath_);
		static nlohmann::json ReadPlainTextJSONFile(const std::string& filePath_);
		static nlohmann::json ReadBinaryJSONFile(const std::string& filePath_);
		
		//Returns true when file write was successful
		static bool WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_ = WriteType::Append);
		static bool WriteToBinaryFile(const std::string& filePath_, const std::vector<uint8_t>& data_, WriteType type_ = WriteType::Overwrite);
		static bool WriteJSONToPlainTextFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_ = WriteType::Overwrite);
		static bool WriteJSONToBinaryFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_ = WriteType::Overwrite);

		static std::string GetFileNameFromPath(const std::string& path_);

		static std::string GetPersistentDataDir();

		static bool CreateFile(const std::string& path_);
		static bool CreateDir(const std::string& path_);

	private:
		static std::string RemoveFileNameFromPath(const std::string& path_);
	};
}

#endif //!GADGET_FILE_SYSTEM_H