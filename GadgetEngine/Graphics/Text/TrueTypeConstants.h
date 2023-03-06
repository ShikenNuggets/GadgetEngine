#ifndef GADGET_TRUE_TYPE_CONSTANTS_H
#define GADGET_TRUE_TYPE_CONSTANTS_H

#include <cstdint>

namespace Gadget{
	namespace TTF{
		enum class SimpleGlyphFlags : uint8_t{
			ON_CURVE_POINT = 0x01,
			X_SHORT_VECTOR = 0x02,
			Y_SHORT_VECTOR = 0x03,
			REPEAT_FLAG = 0x08,
			X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10,
			Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20,
			OVERLAP_SIMPLE = 0x40,
			RESERVED = 0x80
		};

		//TrueType Font Table Tags
		constexpr uint32_t DSIG = 1146308935;
		constexpr uint32_t GDEF = 1195656518;
		constexpr uint32_t GSUB = 1196643650;
		constexpr uint32_t JSTF = 1246975046;
		constexpr uint32_t LTSH = 1280594760;
		constexpr uint32_t OS2 = 1330851634;
		constexpr uint32_t PCLT = 1346587732;
		constexpr uint32_t VDMX = 1447316824;
		constexpr uint32_t cmap = 1668112752;
		constexpr uint32_t cvt = 1668707360;
		constexpr uint32_t fpgm = 1718642541;
		constexpr uint32_t gasp = 1734439792;
		constexpr uint32_t glyf = 1735162214;
		constexpr uint32_t hdmx = 1751412088;
		constexpr uint32_t head = 1751474532;
		constexpr uint32_t hhea = 1751672161;
		constexpr uint32_t hmtx = 1752003704;
		constexpr uint32_t kern = 1801810542;
		constexpr uint32_t loca = 1819239265;
		constexpr uint32_t maxp = 1835104368;
		constexpr uint32_t name = 1851878757;
		constexpr uint32_t post = 1886352244;
		constexpr uint32_t prep = 1886545264;
	}

	namespace Utils{
		constexpr inline bool HasFlag(uint8_t byte_, TTF::SimpleGlyphFlags flag_){
			return (byte_ & static_cast<uint8_t>(flag_));
		}
	}
}

#endif //!GADGET_TRUE_TYPE_CONSTANTS_H