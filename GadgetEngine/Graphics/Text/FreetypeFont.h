#ifndef GADGET_FREETYPE_FONT_H
#define GADGET_FREETYPE_FONT_H

#include <map>

#include "Graphics/FontInfo.h"
#include "Graphics/Loaders/FreetypeFontLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	struct FreetypeFontCharacter{
		FreetypeFontCharacter(size_t width_, size_t rows_, size_t pitch_, size_t left_, size_t top_, size_t advanceX_, void* oldBufferToCopy_) : width(width_), rows(rows_), pitch(pitch_), left(left_), top(top_), advanceX(advanceX_){
			if(oldBufferToCopy_ != nullptr){
				bitmapBuffer = std::vector<unsigned char>((unsigned char*)oldBufferToCopy_, (unsigned char*)oldBufferToCopy_ + (rows * pitch));
			}
		}

		size_t width;
		size_t rows;
		size_t pitch;
		size_t left;
		size_t top;
		size_t advanceX;
		std::vector<unsigned char> bitmapBuffer;
	};

	class FreetypeFont : public Resource{
	public:
		FreetypeFont() : characters(), fontInfo(nullptr){}

		virtual ~FreetypeFont() override{}

		void AddCharacter(char c_, const FreetypeFontCharacter& ffc_){
			characters.emplace(c_, ffc_);
		}

		void SetFontInfo(FontInfo* fontInfo_){
			GADGET_BASIC_ASSERT(fontInfo_ != nullptr);
			fontInfo = fontInfo_;
		}

		const std::map<char, FreetypeFontCharacter>& GetCharacters() const{ return characters; }
		FontInfo* GetFontInfo() const{ return fontInfo; }

	private:
		std::map<char, FreetypeFontCharacter> characters;
		FontInfo* fontInfo;
	};

	class FreetypeFontResourceContainer : public ResourceContainer{
	public:
		FreetypeFontResourceContainer(const std::string& path_) : path(path_){}

		virtual Resource* LoadResource() override{
			FreetypeFontLoader ffl = FreetypeFontLoader();
			return ffl.LoadFont(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(FreetypeFont);
		}

	private:
		std::string path;
	};
}

#endif //!GADGET_FREETYPE_FONT_H