#include "FreetypeFontLoader.h"

#include "App.h"
#include "Graphics/Text/FreetypeFont.h"

using namespace Gadget;

FreetypeFontLoader::FreetypeFontLoader(){
	FT_Error err = FT_Init_FreeType(&ftLib);
	GADGET_ASSERT(err == FT_Err_Ok, "Could not initialize FreeType! FreeType Error Code: " + std::to_string(err));
}

FreetypeFontLoader::~FreetypeFontLoader(){
	FT_Error err = FT_Done_FreeType(ftLib);
	GADGET_ASSERT(err == FT_Err_Ok, "Could not cleanup FreeType! FreeType Error Code: " + std::to_string(err));
}

FreetypeFont* FreetypeFontLoader::LoadFont(const std::string& filePath_){
	GADGET_BASIC_ASSERT(ftLib != nullptr);
	if(ftLib == nullptr){
		return nullptr;
	}

	FT_Face fontFace = nullptr;
	FT_Error err = FT_New_Face(ftLib, filePath_.c_str(), 0, &fontFace);
	GADGET_ASSERT(err == FT_Err_Ok && fontFace != nullptr, "Could not load font face! FreeType Error Code: " + std::to_string(err));

	//Set size to load glyphs as
	err = FT_Set_Pixel_Sizes(fontFace, 0, 128);
	GADGET_ASSERT(err == FT_Err_Ok, "Could not set FreeType pixel sizes! FreeType Error Code: " + std::to_string(err));

	FreetypeFont* font = new FreetypeFont();

	//Load the first 128 ASCII characters
	//TODO - Unicode?
	for(unsigned char c = 0; c < 128; c++) {  
		//Load character glyph 
		err = FT_Load_Char(fontFace, c, FT_LOAD_RENDER);
		GADGET_ASSERT(err == FT_Err_Ok, "Could not load glyph for character " + static_cast<char>(c) + std::string("! FreeType Error Code" + std::to_string(err)));
		
		if(fontFace->glyph->bitmap.buffer == nullptr){
			continue;
		}

		FreetypeFontCharacter character = FreetypeFontCharacter(
			fontFace->glyph->bitmap.width,
			fontFace->glyph->bitmap.rows,
			fontFace->glyph->bitmap.pitch,
			fontFace->glyph->bitmap_left,
			fontFace->glyph->bitmap_top,
			fontFace->glyph->advance.x,
			fontFace->glyph->bitmap.buffer
		);

		font->AddCharacter(c, character);
	}
	
	//Clear out the font face since we don't need it anymore
	FT_Done_Face(fontFace);

	font->SetFontInfo(App::GetRenderer().GenerateAPIFontInfo(*font)); //TODO - This feels like a circular dependency
	return font;
}