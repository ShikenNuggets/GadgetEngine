#include "Win32_Utils.h"

#include <comdef.h>
#include <dwmapi.h>
#include <ShlObj_core.h>
#include <WinUser.h>

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

void Win32_Utils::TryApplyImmersiveDarkMode(uint64_t hwnd_){
	HWND hwnd = reinterpret_cast<HWND>(hwnd_);
	GADGET_BASIC_ASSERT(IsWindow(hwnd));

	BOOL useDarkMode = TRUE;
	const HRESULT result = DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
	if(!SUCCEEDED(result)){
		//Main reason this wouldn't work is on older versions of Windows (Immersive Dark Mode is only officially supported in Windows 11)
		//We could be nice and try to find a way to support this feature on older Windows versions, but I'm not overly concerned about that
		const _com_error err(result);
		const std::wstring wStr = err.ErrorMessage();
		const std::string str = std::string(wStr.begin(), wStr.end());

		GADGET_LOG_WARNING(SID("WIN32"), "Could not apply Immersive Dark Mode: DWM Error: " + str);
	}
}