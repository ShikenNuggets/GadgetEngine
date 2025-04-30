#include "PlatformUtils.h"

#ifdef GADGET_PLATFORM_WIN32
	#include "Platform/Windows/Win32_Utils.h"
	#define CURRENT_PLATFORM_UTILS Win32_Utils
#else
	static_assert(false, "No PlatformUtils implementation defined for the current platform!");
#endif

using namespace Gadget;

std::string PlatformUtils::GetUserDocumentsPath(){
	return CURRENT_PLATFORM_UTILS::GetUserDocumentsPath();
}

void PlatformUtils::SetConsoleColorWhite(){
	CURRENT_PLATFORM_UTILS::SetConsoleColorWhite();
}

void PlatformUtils::SetConsoleColorRed(){
	CURRENT_PLATFORM_UTILS::SetConsoleColorRed();
}

void PlatformUtils::SetConsoleColorYellow(){
	CURRENT_PLATFORM_UTILS::SetConsoleColorYellow();
}

void PlatformUtils::SetConsoleColorGreen(){
	CURRENT_PLATFORM_UTILS::SetConsoleColorGreen();
}

void PlatformUtils::SetConsoleColorBlue(){
	CURRENT_PLATFORM_UTILS::SetConsoleColorBlue();
}

void PlatformUtils::OutputToDebuggerConsole(const std::string& output){
	CURRENT_PLATFORM_UTILS::OutputToDebuggerConsole(output);
}

void PlatformUtils::TryApplyImmersiveDarkMode(uint64_t hwnd_){
	CURRENT_PLATFORM_UTILS::TryApplyImmersiveDarkMode(hwnd_);
}

std::string PlatformUtils::BrowseForFolder(uint64_t hwnd_, const wchar_t* dialogTitle_){
	return CURRENT_PLATFORM_UTILS::BrowseForFolder(hwnd_, dialogTitle_);
}

ErrorCode PlatformUtils::ShowWindow(uint64_t hwnd_){
	return CURRENT_PLATFORM_UTILS::ShowWindow(hwnd_);
}

ErrorCode PlatformUtils::BringWindowToForeground(uint64_t hwnd_){
	return CURRENT_PLATFORM_UTILS::BringWindowToForeground(hwnd_);
}

ErrorCode PlatformUtils::OpenFileInDefaultApplication(const std::string& filePath_, const std::string& args_){
	return CURRENT_PLATFORM_UTILS::OpenFileInDefaultApplication(filePath_, args_);
}

uint64_t PlatformUtils::GetWindowOfRunningApplication(const std::string& windowName_, const std::string& subTitle_){
	return CURRENT_PLATFORM_UTILS::GetWindowOfRunningApplication(windowName_, subTitle_);
}
