#ifndef GADGET_FREETYPE_FONT_H
#define GADGET_FREETYPE_FONT_H

#include <map>

#include "Graphics/FontInfo.h"
#include "Graphics/Loaders/FreetypeFontLoader.h"
#include "Resource/Resource.h"

namespace Gadget{
	struct FreetypeFontCharacter{
		FreetypeFontCharacter(size_t width_, size_t rows_, int pitch_, int left_, int top_, long advanceX_, unsigned char* oldBufferToCopy_) : width(width_), rows(rows_), pitch(pitch_), left(left_), top(top_), advanceX(advanceX_){
			if(oldBufferToCopy_ != nullptr){
				bitmapBuffer.reserve(rows * pitch);
				std::copy(oldBufferToCopy_, oldBufferToCopy_ + (rows * pitch), std::back_inserter(bitmapBuffer));
			}
		}

		size_t width;
		size_t rows;
		int pitch;
		int left;
		int top;
		long advanceX;
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

		static constexpr const char* typeName = "FreetypeFont";

		virtual size_t SizeInBytes() const override{ return sizeof(*this); } //TODO - This isn't quite right

		const std::unordered_map<char, FreetypeFontCharacter>& GetCharacters() const{ return characters; }
		FontInfo* GetFontInfo() const{ return fontInfo; }

	private:
		std::unordered_map<char, FreetypeFontCharacter> characters;
		FontInfo* fontInfo;
	};

	class FreetypeFontResourceContainer : public ResourceContainer{
	public:
		FreetypeFontResourceContainer(const std::string& path_) : ResourceContainer(FreetypeFont::typeName, path_){}

		virtual Resource* LoadResource() override{
			FreetypeFontLoader ffl = FreetypeFontLoader();
			return ffl.LoadFont(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(FreetypeFont);
		}
	};
}

#endif //!GADGET_FREETYPE_FONT_H