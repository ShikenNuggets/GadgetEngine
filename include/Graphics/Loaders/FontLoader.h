#ifndef GADGET_FONT_LOADER_H
#define GADGET_FONT_LOADER_H

#include <string>
#include <vector>

#include "Math/Vector.h"

namespace Gadget{
	//Forward Declaration
	class Font;
	struct GlyphData;

	class FontLoader{
	public:
		Font* LoadFont(const std::string& filePath_);

	private:
		void LoadCmapTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadGlyfTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadHeadTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadHheaTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadHmtxTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadLocaTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadMaxpTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadNameTable(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadOS2Table(const std::vector<uint8_t>& data_, size_t tableOffset_);
		void LoadPostTable(const std::vector<uint8_t>& data_, size_t tableOffset_);

		int16_t indexToLocFormat = 0;
		size_t numGlyphs = 0;
		std::vector<uint32_t> offsets;
		std::vector<GlyphData> glypthDatas;
	};
}

#endif //!GADGET_FONT_LOADER_H