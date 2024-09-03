#ifndef GADGET_FONT_H
#define GADGET_FONT_H

#include "Graphics/Loaders/FontLoader.h"
#include "Graphics/Text/GlyphData.h"
#include "Resource/Resource.h"

namespace Gadget{
	class Font : public Resource{
	public:
		Font(const std::vector<GlyphData>& glyphs_) : glyphs(glyphs_){}
		virtual ~Font() override{}

		static constexpr const char* typeName = "Font";

		virtual size_t SizeInBytes() const override{
			size_t size = sizeof(*this);
			for(const auto& g : glyphs){
				size += g.SizeInBytes();
			}

			return size;
		}

		std::vector<Vector2> CalculatePolygonForGlyph(unsigned char glyph_);
		std::vector<std::vector<Vector2>> CalculatePolygonsForString(const std::string& text_);

	private:
		std::vector<GlyphData> glyphs;
	};

	class FontResourceContainer : public ResourceContainer{
	public:
		FontResourceContainer(const std::string& path_) : ResourceContainer(Font::typeName, path_){}

		virtual Resource* LoadResource() override{
			FontLoader fl = FontLoader();
			return fl.LoadFont(path);
		}

		virtual const std::type_info& GetResourceTypeInfo() override{
			return typeid(Font);
		}
	};
}

#endif //!GADGET_FONT_H