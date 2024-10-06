#ifndef GADGET_WIN32_UTILS_H
#define GADGET_WIN32_UTILS_H

#include <string>

#include "GadgetEnums.h"

namespace Gadget{
	namespace Win32_Utils{
		struct EnumerateWindowsInfo{
			uint64_t hwnd;
			const wchar_t* windowTitle;
			const wchar_t* subTitle;
		};

		std::string GetUserDocumentsPath();

		void SetConsoleColorWhite();
		void SetConsoleColorRed();
		void SetConsoleColorYellow();
		void SetConsoleColorGreen();
		void SetConsoleColorBlue();

		void OutputToDebuggerConsole(const std::string& output);

		void TryApplyImmersiveDarkMode(uint64_t hwnd_);

		std::string BrowseForFolder(uint64_t hwnd_, const wchar_t* dialogTitle_);

		ErrorCode ShowWindow(uint64_t hwnd_);
		ErrorCode BringWindowToForeground(uint64_t hwnd_);
		ErrorCode OpenFileInDefaultApplication(const std::string& filePath_, const std::string& args_ = "");

		uint64_t GetWindowOfRunningApplication(const std::string& windowName_, const std::string& subTitle_ = "");
	};
}

#endif //!GADGET_WIN32_UTILS_H