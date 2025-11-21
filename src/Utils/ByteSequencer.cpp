#include "ByteSequencer.h"

#include "Debug.h"
#include "Utils/Utils.h"

using namespace Gadget;

ByteSequencer::ByteSequencer(const std::vector<uint8_t>& buffer_, size_t initialOffset_, bool littleEndian_) : buffer(buffer_), currentOffset(initialOffset_), littleEndian(littleEndian_){
	GADGET_BASIC_ASSERT(!buffer_.empty());
	GADGET_BASIC_ASSERT(initialOffset_ < buffer_.size());
}

uint8_t ByteSequencer::Sequence1Byte(){
	if(currentOffset >= buffer.size()){
		return 0;
	}

	uint8_t val = buffer[currentOffset];
	currentOffset += 1;
	return val;
}

uint16_t ByteSequencer::Sequence2Bytes(){
	uint16_t val = Utils::MergeTwoBytes(buffer, currentOffset, littleEndian);
	currentOffset += 2;
	return val;
}

uint32_t ByteSequencer::Sequence4Bytes(){
	uint32_t val = Utils::MergeFourBytes(buffer, currentOffset, littleEndian);
	currentOffset += 4;
	return val;
}

uint64_t ByteSequencer::Sequence8Bytes(){
	uint64_t val = Utils::MergeEightBytes(buffer, currentOffset, littleEndian);
	currentOffset += 8;
	return val;
}

void ByteSequencer::ResetOffset(size_t newOffset_){
	currentOffset = newOffset_;
}