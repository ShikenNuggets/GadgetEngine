#include "Win32_Utils.h"

#include <ShlObj_core.h>

#include "Debug.h"

using namespace Gadget;

HANDLE gConsole = GetStdHandle(STD_OUTPUT_HANDLE);

static inline bool IsConsoleHandleValid(){ return gConsole != nullptr && gConsole != INVALID_HANDLE_VALUE; }

std::string Win32_Utils::GetUserDocumentsPath(){
	WCHAR* path = nullptr;
	auto result = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path);
	if(!SUCCEEDED(result) || path == nullptr){
		return "";
	}
	
	const std::wstring ws(path);

	std::string resultStr;
	for(const auto& wchar : ws){
		resultStr += static_cast<char>(wchar); //TODO - Unicode - This seems silly but it avoids a compiler warning
	}
	return resultStr;
}

void Win32_Utils::SetConsoleColorWhite(){ if(IsConsoleHandleValid()){ SetConsoleTextAttribute(gConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); }}

void Win32_Utils::SetConsoleColorRed(){ if(IsConsoleHandleValid()){ SetConsoleTextAttribute(gConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); }}

void Win32_Utils::SetConsoleColorYellow(){ if(IsConsoleHandleValid()){ SetConsoleTextAttribute(gConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); }}

void Win32_Utils::SetConsoleColorGreen(){ if(IsConsoleHandleValid()){ SetConsoleTextAttribute(gConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); }}

void Win32_Utils::SetConsoleColorBlue(){ if(IsConsoleHandleValid()){ SetConsoleTextAttribute(gConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); }}

void Win32_Utils::OutputToDebuggerConsole(const std::string& output_){
	OutputDebugStringA(output_.c_str());
}