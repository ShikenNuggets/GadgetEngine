#ifndef GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H
#define GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H

#include "Core/FileSystem.h"
#include "Platform/Windows/Win32_Utils.h"

namespace Gadget::Workbench::VisualStudio{
	static inline constexpr const char* gPremakeTemplateFile = "premake-template.lua";
	static inline constexpr const char* gPremakeFile = "premake5.lua";

	inline ErrorCode OpenSolution(const String& projectName_, const String& projectPath_){
		uint64_t hwnd = Win32_Utils::GetWindowOfRunningApplication("Microsoft Visual Studio", projectName_);
		if(hwnd == NULL){
			const ErrorCode err = Win32_Utils::OpenFileInDefaultApplication(projectPath_ + FileSystem::PathSeparator + projectName_);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			return err;
		}else{
			ErrorCode err = Win32_Utils::ShowWindow(hwnd);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			if(err != ErrorCode::OK){
				return err;
			}

			err = Win32_Utils::BringWindowToForeground(hwnd);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			return err;
		}
	}

	inline ErrorCode GenerateSolution(const String& projectName_, const String& projectPath_){
		String str = FileSystem::ReadFileToString(gPremakeTemplateFile);
		str.FindAndReplace("__MACRO_PROJECT_NAME__", projectName_);
		str.FindAndReplace("__MACRO_PROJECT_PATH__", projectPath_);
		str.FindAndReplace("__WORKBENCH_DIR__", FileSystem::WorkingDir());
		ErrorCode err = FileSystem::WriteToFile(gPremakeFile, str, FileSystem::WriteType::Overwrite);
		if(err != ErrorCode::OK){
			return err;
		}

		return Win32_Utils::OpenFileInDefaultApplication("premake\\premake5.exe", "vs2022");
	}
}

#endif //!GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H