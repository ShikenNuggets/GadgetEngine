#ifndef GADGET_FILE_SYSTEM_H
#define GADGET_FILE_SYSTEM_H

#include <fstream>
#include <string>
#include <vector>

namespace Gadget{
	class FileSystem{
	public:
		enum class WriteType{
			Append,
			Clear,
			Overwrite,
		};

		static bool FileExists(const std::string& filePath_);

		static std::vector<std::string> ReadFile(const std::string& filePath_);
		static std::string ReadFileToString(const std::string& filePath_);
		
		static void WriteToFile(const std::string& filePath_, const std::string& content_, WriteType type_ = WriteType::Append);
	};
}

#endif //!GADGET_FILE_SYSTEM_H