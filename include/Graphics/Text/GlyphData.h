#ifndef GADGET_GLYPH_DATA_H
#define GADGET_GLYPH_DATA_H

#include <vector>

namespace Gadget{
	struct GlyphContourPoint{
		bool isOnCurve = false;
		bool shortXVector = false;
		bool shortYVector = false;
		bool flagRepeats = false;
		bool xRepeats = false;
		bool yRepeats = false;
		bool xIsPositive = false;
		bool yIsPositive = false;
		bool contoursOverlap = false;

		int16_t xCoordinate = 0;
		int16_t yCoordinate = 0;
	};

	struct GlyphData{
		std::vector<uint16_t> endPtsOfContours;
		std::vector<uint8_t> instructions;
		std::vector<uint8_t> flags;
		std::vector<int16_t> xCoordinates;
		std::vector<int16_t> yCoordinates;

		std::vector<GlyphContourPoint> points;

		constexpr inline size_t SizeInBytes() const{
			size_t size = sizeof(*this);
			size += sizeof(uint16_t) * endPtsOfContours.capacity();
			size += sizeof(uint8_t) * instructions.capacity();
			size += sizeof(uint8_t) * flags.capacity();
			size += sizeof(int16_t) * xCoordinates.capacity();
			size += sizeof(int16_t) * yCoordinates.capacity();
			size += sizeof(GlyphContourPoint) * points.capacity();

			return size;
		}
	};
}

#endif //!GADGET_GLYPH_DATA_H