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

static int CallBack(HWND hwnd, UINT uMsg, [[maybe_unused]] LPARAM lParam, LPARAM lpData){
	if(uMsg == BFFM_INITIALIZED && lpData != 0){
		const std::string tmp = reinterpret_cast<const char*>(lpData);
		GADGET_LOG(SID("BROWSER"), tmp);
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

std::string Win32_Utils::BrowseForFolder(uint64_t hwnd_, const wchar_t* dialogTitle_){
	HWND hwnd = reinterpret_cast<HWND>(hwnd_);
	GADGET_BASIC_ASSERT(IsWindow(hwnd));

	BROWSEINFO browseInfo{};
	browseInfo.lpszTitle = dialogTitle_;
	browseInfo.hwndOwner = hwnd;
	browseInfo.lpfn = CallBack;
	browseInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pidl = SHBrowseForFolder(&browseInfo);
	if(pidl == nullptr){
		return "";
	}

	TCHAR path[MAX_PATH]{};
	const BOOL result = SHGetPathFromIDList(pidl, path);
	if(result == FALSE){
		return "";
	}

	std::wstring wstr = path;
	return std::string(wstr.begin(), wstr.end()) + FileSystem::PathSeparator;
}

ErrorCode Win32_Utils::ShowWindow(uint64_t hwnd_){
	if(hwnd_ == 0){
		return ErrorCode::Invalid_Args;
	}

	HWND hwnd = reinterpret_cast<HWND>(hwnd_);
	GADGET_BASIC_ASSERT(IsWindow(hwnd));
	if(IsWindow(hwnd) == FALSE){
		return ErrorCode::Invalid_Args;
	}

	const BOOL result = ShowWindow(hwnd, SW_RESTORE);
	if(result == FALSE){
		return ErrorCode::Win32_Error;
	}

	return ErrorCode::OK;
}

ErrorCode Win32_Utils::BringWindowToForeground(uint64_t hwnd_){
	if(hwnd_ == 0){
		return ErrorCode::Invalid_Args;
	}

	HWND hwnd = reinterpret_cast<HWND>(hwnd_);
	GADGET_BASIC_ASSERT(IsWindow(hwnd));
	if(IsWindow(hwnd) == FALSE){
		return ErrorCode::Invalid_Args;
	}

	const BOOL result = SetForegroundWindow(hwnd);
	if(result == FALSE){
		return ErrorCode::Win32_Error;
	}

	return ErrorCode::OK;
}

ErrorCode Win32_Utils::OpenFileInDefaultApplication(const std::string& filePath_, const std::string& args_){
	GADGET_BASIC_ASSERT(FileSystem::FileExists(filePath_));
	if(!FileSystem::FileExists(filePath_)){
		return ErrorCode::Invalid_Args;
	}

	const std::wstring wFilePath = std::wstring(filePath_.begin(), filePath_.end());
	const std::wstring wArgs = std::wstring(args_.begin(), args_.end());
	const auto result = reinterpret_cast<INT_PTR>(ShellExecute(nullptr, nullptr, wFilePath.c_str(), wArgs.c_str(), nullptr, SW_SHOW));
	if(result > 32){
		return ErrorCode::OK;
	}

	switch(result){
		case 0:						[[fallthrough]];
		case SE_ERR_ASSOCINCOMPLETE:[[fallthrough]];
		case SE_ERR_DDEBUSY:		[[fallthrough]];
		case SE_ERR_DDEFAIL:		[[fallthrough]];
		case SE_ERR_DDETIMEOUT:
			return ErrorCode::Win32_Error;

		case ERROR_FILE_NOT_FOUND:	[[fallthrough]];
		case ERROR_PATH_NOT_FOUND:	[[fallthrough]];
		case SE_ERR_DLLNOTFOUND:
			return ErrorCode::Invalid_Args;

		case ERROR_ACCESS_DENIED:	[[fallthrough]];
		case SE_ERR_SHARE:
			return ErrorCode::Win32_FileIO_PermissionsError;

		case SE_ERR_NOASSOC:
			return ErrorCode::Win32_NoAssociationError;

		case SE_ERR_OOM:
			return ErrorCode::Out_Of_Memory;

		default:
			break;
	}
	
	GADGET_ASSERT(false, "Unhandled error in Win32_Utils::OpenFileInDefaultApplication");
	return ErrorCode::Unknown;
}

uint64_t Win32_Utils::GetWindowOfRunningApplication(const std::string& windowName_, const std::string& subTitle_){
	std::wstring wNameStr = { windowName_.begin(), windowName_.end() };
	std::wstring wSubTitleStr = { subTitle_.begin(), subTitle_.end() };
	
	Utils::ToFuzzyCompareStrInPlace(wNameStr);
	Utils::ToFuzzyCompareStrInPlace(wSubTitleStr);

	EnumerateWindowsInfo info{};
	info.hwnd = 0;
	info.windowTitle = wNameStr.c_str();
	info.subTitle = nullptr;
	if(!wSubTitleStr.empty()){
		info.subTitle = wSubTitleStr.c_str();
	}

	EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
		WCHAR title[256]{ '\0' };
		GetWindowText(hWnd, title, 256);

		const std::wstring wTitleStr = Utils::CreateFuzzyCompareStr(title);

		EnumerateWindowsInfo* info = reinterpret_cast<EnumerateWindowsInfo*>(lParam);
		if(wcsstr(wTitleStr.c_str(), info->windowTitle) != nullptr){
			if(info->subTitle == nullptr || wcsstr(wTitleStr.c_str(), info->subTitle) != nullptr){
				info->hwnd = reinterpret_cast<uint64_t>(hWnd);
				return FALSE;
			}
		}

		return TRUE;
	}, reinterpret_cast<LPARAM>(&info));

	GADGET_BASIC_ASSERT(info.hwnd == 0 || IsWindow(reinterpret_cast<HWND>(info.hwnd)));
	return info.hwnd;
}