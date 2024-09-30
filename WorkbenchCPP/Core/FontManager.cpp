#include "FontManager.h"

#include <imgui.h>

using namespace Gadget;
using namespace Workbench;

FontManager::FontManager(){
	GADGET_BASIC_ASSERT(FileSystem::FileExists("Resources/Fonts/arial.ttf"));
	Add(SID("Arial"), "Resources/Fonts/arial.ttf");
}

void FontManager::Add(StringID name_, const String& filePath_){
	fonts.Add(name_, ImGui::GetIO().Fonts->AddFontFromFileTTF(filePath_.Value(), 18));
}

ImFont* FontManager::Get(StringID name_) const{
	GADGET_BASIC_ASSERT(fonts.Contains(name_));
	if(!fonts.Contains(name_)){
		return nullptr;
	}

	return fonts[name_];
}