#include "Win32_Utils.h"

#include <ShlObj_core.h>

#include "Debug.h"

using namespace Gadget;

std::string Win32_Utils::GetUserDocumentsPath(){
	WCHAR* path;
	auto result = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path);
	if(!SUCCEEDED(result)){
		return "";
	}
	
	std::wstring ws(path);

	std::string resultStr;
	for(const auto& wchar : ws){
		resultStr += static_cast<char>(wchar); //TODO - Unicode - This seems silly but it avoids a compiler warning
	}
	return resultStr;
}