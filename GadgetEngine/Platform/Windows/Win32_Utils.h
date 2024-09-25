#ifndef GADGET_WIN32_UTILS_H
#define GADGET_WIN32_UTILS_H

#include <string>

namespace Gadget{
	namespace Win32_Utils{
		std::string GetUserDocumentsPath();

		void SetConsoleColorWhite();
		void SetConsoleColorRed();
		void SetConsoleColorYellow();
		void SetConsoleColorGreen();
		void SetConsoleColorBlue();

		void OutputToDebuggerConsole(const std::string& output);

		void TryApplyImmersiveDarkMode(uint64_t hwnd_);

		std::string BrowseForFolder(uint64_t hwnd_, const wchar_t* dialogTitle_);
	};
}

#endif //!GADGET_WIN32_UTILS_H