#ifndef GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H
#define GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H

#include "Core/FileSystem.h"
#include "Platform/PlatformUtils.h"

namespace Gadget::Workbench::VisualStudio{
	static inline constexpr const char* gPremakeTemplateFile = "premake.template";
	static inline constexpr const char* gPremakeFile = "premake5.lua";
	static inline constexpr const char* gVsSolutionExtension = ".sln";
	static inline constexpr const char* gMainTemplateFile = "main.template";
	static inline constexpr const char* gMainFile = "Main.cpp";

	inline ErrorCode OpenSolution(const String& projectName_, const String& projectPath_){
		uint64_t hwnd = PlatformUtils::GetWindowOfRunningApplication("Microsoft Visual Studio", projectName_);
		if(hwnd == NULL){
			const ErrorCode err = PlatformUtils::OpenFileInDefaultApplication(projectPath_ + FileSystem::PathSeparator + projectName_ + gVsSolutionExtension);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			return err;
		}else{
			ErrorCode err = PlatformUtils::ShowWindow(hwnd);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			if(err != ErrorCode::OK){
				return err;
			}

			err = PlatformUtils::BringWindowToForeground(hwnd);
			GADGET_BASIC_ASSERT(err == ErrorCode::OK);
			return err;
		}
	}

	inline ErrorCode GenerateSolution(const String& projectName_, const String& projectPath_){
		String str = FileSystem::ReadFileToString(gPremakeTemplateFile);
		str.FindAndReplace("__MACRO_PROJECT_NAME__", projectName_);

		String projectPathDir = projectPath_;
		projectPathDir.FindAndReplace(String(FileSystem::PathSeparator), String(FileSystem::PathSeparator) + FileSystem::PathSeparator);
		str.FindAndReplace("__MACRO_PROJECT_PATH__", projectPathDir);

		String workingDir = FileSystem::WorkingDir();
		workingDir.FindAndReplace(String(FileSystem::PathSeparator), String(FileSystem::PathSeparator) + FileSystem::PathSeparator);

		str.FindAndReplace("__MACRO_WORKBENCH_DIR__", workingDir);
		ErrorCode err = FileSystem::WriteToFile(gPremakeFile, str, FileSystem::WriteType::Overwrite);
		if(err != ErrorCode::OK){
			return err;
		}

		str = FileSystem::ReadFileToString(gMainTemplateFile);
		str.FindAndReplace("__MACRO_PROJECT_NAME__", projectName_);
		err = FileSystem::WriteToFile(projectPath_ + FileSystem::PathSeparator + gMainFile, str, FileSystem::WriteType::Overwrite);
		if(err != ErrorCode::OK){
			return err;
		}

		return PlatformUtils::OpenFileInDefaultApplication("premake\\premake5.exe", "vs2022");
	}
}

#endif //!GADGET_WORKBENCH_UTILS_VISUAL_STUDIO_HELPER_H