#include "FontLoader.h"

#include <array>

#include "Core/FileSystem.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Text/GlyphData.h"
#include "Graphics/Text/TrueTypeConstants.h"
#include "Utils/ByteSequencer.h"
#include "Utils/Utils.h"

using namespace Gadget;

Font* FontLoader::LoadFont(const std::string& filePath_){
	auto data = FileSystem::ReadBinaryFile(filePath_);
	data.shrink_to_fit();

	ByteSequencer bs = ByteSequencer(data, 0, false);

	uint32_t sfntVersion = bs.Sequence4Bytes();
	GADGET_BASIC_ASSERT(sfntVersion == 0x00010000 || sfntVersion == 0x4F54544F);

	uint16_t numTables = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t searchRange = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t entrySelector = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t rangeShift = bs.Sequence2Bytes();

	std::map<uint32_t, uint32_t> tableOffsets;

	for(uint16_t i = 0; i < numTables; i++){
		uint32_t tagInt = bs.Sequence4Bytes();
		[[maybe_unused]] uint32_t checksum = bs.Sequence4Bytes();
		uint32_t offset = bs.Sequence4Bytes();
		[[maybe_unused]] uint32_t length = bs.Sequence4Bytes();

		tableOffsets.emplace(tagInt, offset);

		//switch(tagInt){
		//	case TTF::DSIG:
		//	case TTF::GDEF:
		//	case TTF::GSUB:
		//	case TTF::JSTF:
		//	case TTF::LTSH:
		//	case TTF::PCLT:
		//	case TTF::VDMX:
		//		break; //Ignore OpenType tables for now. TODO - We should eventually acknowledge these
		//	case TTF::cmap:
		//		LoadCmapTable(data, offset);
		//		break;
		//	case TTF::cvt:
		//	case TTF::fpgm:
		//	case TTF::gasp:
		//		break;
		//	case TTF::glyf:
		//		LoadGlyfTable(data, offset);
		//		break;
		//	case TTF::hdmx:
		//		break;
		//	case TTF::head:
		//		LoadHeadTable(data, offset);
		//		break;
		//	case TTF::hhea:
		//	case TTF::hmtx:
		//	case TTF::kern:
		//	case TTF::loca:
		//		LoadLocaTable(data, offset);
		//		break;
		//	case TTF::maxp:
		//		LoadMaxpTable(data, offset);
		//		break;
		//	case TTF::name:
		//	case TTF::OS2:
		//	case TTF::post:
		//	case TTF::prep:
		//		break;
		//	default:
		//		break;
		//}
	}

	//Load the tables in a specific order
	GADGET_BASIC_ASSERT(Utils::ContainsKey(tableOffsets, TTF::head));
	GADGET_BASIC_ASSERT(Utils::ContainsKey(tableOffsets, TTF::maxp));
	GADGET_BASIC_ASSERT(Utils::ContainsKey(tableOffsets, TTF::loca));
	GADGET_BASIC_ASSERT(Utils::ContainsKey(tableOffsets, TTF::glyf));

	LoadHeadTable(data, tableOffsets[TTF::head]);
	LoadMaxpTable(data, tableOffsets[TTF::maxp]);
	LoadLocaTable(data, tableOffsets[TTF::loca]);
	LoadGlyfTable(data, tableOffsets[TTF::glyf]);

	return new Font(glypthDatas);
}

void FontLoader::LoadCmapTable(const std::vector<uint8_t>& data_, size_t tableOffset_){
	ByteSequencer bs = ByteSequencer(data_, tableOffset_, false);

	[[maybe_unused]] uint16_t version = bs.Sequence2Bytes();
	uint16_t numSubTables = bs.Sequence2Bytes();

	for(size_t i = 0; i < numSubTables; i++){
		[[maybe_unused]] uint16_t platformID = bs.Sequence2Bytes();
		[[maybe_unused]] uint16_t platformSpecificID = bs.Sequence2Bytes();
		uint32_t offset = bs.Sequence4Bytes();

		uint16_t format = Utils::MergeTwoBytes(data_, tableOffset_ + offset, false);
		[[maybe_unused]] uint16_t length = Utils::MergeTwoBytes(data_, tableOffset_ + offset + 2, false);
		[[maybe_unused]] uint16_t language = Utils::MergeTwoBytes(data_, tableOffset_ + offset + 4, false);

		if(format == 0){ //Byte encoding table
			std::array<uint8_t, 256> glyphIdArray = std::array<uint8_t, 256>();
			for(size_t j = 0; j < 256; j++){
				glyphIdArray[j] = data_[offset + 6 + j];
			}
		}else if(format == 2){ //High-byte mapping through table
		}else if(format == 4){ //Segment mapping to delta values
		}else if(format == 6){ //Trimmed table mapping
		}else if(format == 8){ //Mixed 16-bit and 32-bit coverage
		}else if(format == 10){ //Trimmed array
		}else if(format == 12){ //Segmented coverage
		}else if(format == 13){ //Many-to-one range mappings
		}else if(format == 14){ //Unicode Variation Sequences
		}
	}
}

void FontLoader::LoadHeadTable(const std::vector<uint8_t>& data_, size_t tableOffset_){
	ByteSequencer bs = ByteSequencer(data_, tableOffset_, false);

	[[maybe_unused]] uint16_t majorVersion = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t minorVersion = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t fontRevision = bs.Sequence2Bytes();
	[[maybe_unused]] uint32_t checksumAdjustment = bs.Sequence4Bytes();
	[[maybe_unused]] uint32_t magicNumber = bs.Sequence4Bytes();
	[[maybe_unused]] uint16_t flags = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t unitsPerEm = bs.Sequence2Bytes();
	[[maybe_unused]] uint64_t created = bs.Sequence8Bytes();
	[[maybe_unused]] uint64_t modified = bs.Sequence8Bytes();
	[[maybe_unused]] int16_t xMin = bs.Sequence2Bytes();
	[[maybe_unused]] int16_t yMin = bs.Sequence2Bytes();
	[[maybe_unused]] int16_t xMax = bs.Sequence2Bytes();
	[[maybe_unused]] int16_t yMax = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t macStyle = bs.Sequence2Bytes();
	[[maybe_unused]] uint16_t lowestRecPPEM = bs.Sequence2Bytes();
	[[maybe_unused]] int16_t fontDirectionHint = bs.Sequence2Bytes();
	indexToLocFormat = bs.Sequence2Bytes();

	GADGET_BASIC_ASSERT(indexToLocFormat == 0 || indexToLocFormat == 1);

	[[maybe_unused]] int16_t glyphDataFormat = bs.Sequence2Bytes();
}

void FontLoader::LoadGlyfTable(const std::vector<uint8_t>& data_, size_t tableOffset_){
	GADGET_BASIC_ASSERT(numGlyphs == offsets.size());
	for(size_t i = 0; i < numGlyphs; i++){
		size_t offsetToGlyph = tableOffset_ + offsets[i];
		ByteSequencer bs = ByteSequencer(data_, offsetToGlyph, false);

		int16_t numberOfContours = bs.Sequence2Bytes();
		[[maybe_unused]] int16_t xMin = bs.Sequence2Bytes();
		[[maybe_unused]] int16_t yMin = bs.Sequence2Bytes();
		[[maybe_unused]] int16_t xMax = bs.Sequence2Bytes();
		[[maybe_unused]] int16_t yMax = bs.Sequence2Bytes();

		if(numberOfContours < 0){
			GADGET_BASIC_ASSERT(numberOfContours == -1);
			//TODO - Composite glyph
		}else{
			GlyphData gd;
			for(size_t j = 0; j < numberOfContours; j++){
				gd.endPtsOfContours.push_back(bs.Sequence2Bytes());
			}

			size_t instructionLength = bs.Sequence2Bytes();
			for(size_t j = 0; j < instructionLength; j++){
				gd.instructions.push_back(bs.Sequence1Byte());
			}

			size_t numFlags = gd.endPtsOfContours.back();
			for(size_t j = 0; j < numFlags; j++){
				uint8_t flag = bs.Sequence1Byte();

				GlyphContourPoint gcp;
				gcp.isOnCurve = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::ON_CURVE_POINT);
				gcp.shortXVector = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::X_SHORT_VECTOR);
				gcp.shortYVector = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::Y_SHORT_VECTOR);
				gcp.flagRepeats = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::REPEAT_FLAG);
				gcp.xRepeats = gcp.xIsPositive = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR);
				gcp.yRepeats = gcp.yIsPositive = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR);
				gcp.contoursOverlap = Utils::HasFlag(flag, TTF::SimpleGlyphFlags::OVERLAP_SIMPLE);
				gd.points.push_back(gcp);

				if(gcp.flagRepeats){
					size_t numRepeats = bs.Sequence1Byte();
					for(size_t k = 0; k < numRepeats; k++){
						gd.points.push_back(gcp);
						j++;
					}
				}
			}

			int16_t lastXCoord = 0;
			for(auto& p : gd.points){
				if(!p.shortXVector && p.xRepeats){
					p.xCoordinate = lastXCoord;
					continue;
				}

				if(p.shortXVector){
					int16_t xCoord = bs.Sequence1Byte();
					if(p.xIsPositive){
						p.xCoordinate = xCoord;
					}else{
						p.xCoordinate = -xCoord;
					}
				}else{
					p.xCoordinate = bs.Sequence2Bytes();
				}

				lastXCoord = p.xCoordinate;
			}

			int16_t lastYCoord = 0;
			for(auto& p : gd.points){
				if(!p.shortYVector && p.yRepeats){
					p.yCoordinate = lastYCoord;
					continue;
				}

				if(p.shortYVector){
					int16_t yCoord = bs.Sequence1Byte();
					if(p.yIsPositive){
						p.yCoordinate = yCoord;
					}else{
						p.yCoordinate = -yCoord;
					}
				}else{
					[[maybe_unused]] int16_t xCoord = bs.Sequence2Bytes();
				}

				lastYCoord = p.yCoordinate;
			}

			glypthDatas.push_back(gd);
		}
	}
}

void FontLoader::LoadLocaTable(const std::vector<uint8_t>& data_, size_t tableOffset_){
	ByteSequencer bs = ByteSequencer(data_, tableOffset_, false);

	offsets.reserve(numGlyphs);

	if(indexToLocFormat == 0){
		for(size_t i = 0; i < numGlyphs; i++){
			offsets.push_back(bs.Sequence2Bytes() * 2);
		}
	}else{
		for(size_t i = 0; i < numGlyphs; i++){
			offsets.push_back(bs.Sequence4Bytes());
		}
	}

	offsets.shrink_to_fit();
}

void FontLoader::LoadMaxpTable(const std::vector<uint8_t>& data_, size_t tableOffset_){
	ByteSequencer bs = ByteSequencer(data_, tableOffset_, false);

	uint32_t version = bs.Sequence4Bytes();
	numGlyphs = bs.Sequence2Bytes();

	if(version == 0x00010000){
		//TODO - Other data
	}
}