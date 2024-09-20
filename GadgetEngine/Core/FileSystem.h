#ifndef GADGET_FILE_SYSTEM_H
#define GADGET_FILE_SYSTEM_H

#include <string>
#include <vector>

//Hide warnings from external code that we can't/won't modify - WNF
#pragma warning(disable : 26495)	//Uninitialized member variable
#pragma warning(disable : 26800)	//Use of a moved object
#pragma warning(disable : 26819)	//Unnanotated fallthrough in switch statement
#include <nlohmann/json.hpp>
#pragma warning(default : 26495)
#pragma warning(default : 26800)
#pragma warning(default : 26819)

#include "GadgetEnums.h"
#include "Utils/Utils.h"

namespace Gadget{
	//This should ideally be the only place in the code that directly interacts with the file system
	class FileSystem{
	public:
		STATIC_CLASS(FileSystem);

		enum class WriteType : uint8_t{
			Append,
			Clear,
			Overwrite,

			WriteType_MAX //Put new values ABOVE this, nothing should be below
		};

	#ifdef GADGET_PLATFORM_WIN32
		static constexpr char PathSeparator = '\\';
		static constexpr char PathSeparator2 = '/';
	#else
		static constexpr char PathSeparator = '/';
		static constexpr char PathSeparator2 = '\\';
	#endif //GADGET_PLATFORM_WIN32
		
		static bool FileExists(const std::string& filePath_);
		static bool DirExists(const std::string& path_);

		static std::vector<std::string> ReadFile(const std::string& filePath_);
		static std::string ReadFileToString(const std::string& filePath_);
		static std::vector<uint8_t> ReadBinaryFile(const std::string& filePath_);
		static nlohmann::json ReadPlainTextJSONFile(const std::string& filePath_);
		static nlohmann::json ReadBinaryJSONFile(const std::string& filePath_);
		
		[[nodiscard]] static ErrorCode WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_ = WriteType::Append);
		[[nodiscard]] static ErrorCode WriteToBinaryFile(const std::string& filePath_, const std::vector<uint8_t>& data_, WriteType type_ = WriteType::Overwrite);
		[[nodiscard]] static ErrorCode WriteJSONToPlainTextFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_ = WriteType::Overwrite);
		[[nodiscard]] static ErrorCode WriteJSONToBinaryFile(const std::string& filePath_, const nlohmann::json& json_, WriteType type_ = WriteType::Overwrite);

		static constexpr std::string GetFileNameFromPath(const std::string& path_){
			size_t i = path_.rfind(PathSeparator, path_.length());
			if(i != std::string::npos){
				return(path_.substr(i + 1, path_.length() - i));
			}

			i = path_.rfind(PathSeparator2, path_.length());
			if(i != std::string::npos){
				return(path_.substr(i + 1, path_.length() - i));
			}

			return path_;
		}

		static std::string GetPersistentDataDir();

		[[nodiscard]] static ErrorCode CreateFile(const std::string& file_);
		[[nodiscard]] static ErrorCode CreateDir(const std::string& path_);

		static bool IsLastWriteTimeNewer(const std::string& basePath_, const std::string& comparePath_);

		static std::string ConstructFilePath(std::convertible_to<std::string_view> auto&& ...s){
			std::string finalStr = "";
			for(const auto& v : std::initializer_list<std::string_view>{ s... }){
				if(!finalStr.empty()){
					finalStr += PathSeparator;
				}

				finalStr += v;
			}
			return finalStr;
		}

	private:
		static std::string RemoveFileNameFromPath(const std::string& path_);
	};
}

#endif //!GADGET_FILE_SYSTEM_H