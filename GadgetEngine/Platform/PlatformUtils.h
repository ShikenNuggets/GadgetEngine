#ifndef GADGET_PLATFORM_PLATFORM_UTILS_H
#define GADGET_PLATFORM_PLATFORM_UTILS_H

#include <string>

#include "GadgetEnums.h"

namespace Gadget::PlatformUtils{
	extern std::string GetUserDocumentsPath();

	extern void SetConsoleColorWhite();
	extern void SetConsoleColorRed();
	extern void SetConsoleColorYellow();
	extern void SetConsoleColorGreen();
	extern void SetConsoleColorBlue();

	extern void OutputToDebuggerConsole(const std::string& output);

	extern void TryApplyImmersiveDarkMode(uint64_t hwnd_);

	extern std::string BrowseForFolder(uint64_t hwnd_, const wchar_t* dialogTitle_);

	extern ErrorCode ShowWindow(uint64_t hwnd_);
	extern ErrorCode BringWindowToForeground(uint64_t hwnd_);
	extern ErrorCode OpenFileInDefaultApplication(const std::string& filePath_, const std::string& args_ = "");

	extern uint64_t GetWindowOfRunningApplication(const std::string& windowName_, const std::string& subTitle_ = "");
}

#endif //!GADGET_PLATFORM_PLATFORM_UTILS_H