#ifndef GADGET_BYTE_SEQUENCER_H
#define GADGET_BYTE_SEQUENCER_H

#include <vector>

namespace Gadget{
	class ByteSequencer{
	public:
		ByteSequencer(const std::vector<uint8_t>& buffer_, size_t initialOffset_ = 0, bool littleEndian_ = (std::endian::native == std::endian::little));

		uint8_t Sequence1Byte();
		uint16_t Sequence2Bytes();
		uint32_t Sequence4Bytes();
		uint64_t Sequence8Bytes();

		void ResetOffset(size_t newOffset_ = 0);

	private:
		const std::vector<uint8_t>& buffer;
		size_t currentOffset;
		const bool littleEndian;
	};
}

#endif //!GADGET_BYTE_SEQUENCER_H