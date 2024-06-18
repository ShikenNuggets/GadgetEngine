#ifndef GADGET_BITSET_H
#define GADGET_BITSET_H

#include <cstddef>

#include "Data/Array.h"

namespace Gadget{
	class Bitset{
	public:
		union Byte{
			Byte(bool defaultValue_ = false){
				if(defaultValue_){
					byte = 0xFF;
				}else{
					byte = 0;
				}
			}

			unsigned char byte;

			struct{
				bool bit0 : 1;
				bool bit1 : 1;
				bool bit2 : 1;
				bool bit3 : 1;
				bool bit4 : 1;
				bool bit5 : 1;
				bool bit6 : 1;
				bool bit7 : 1;
			} bits;
		};

		Bitset(size_t sizeInBytes, bool defaultValue_ = false) : bytes(sizeInBytes){
			GADGET_BASIC_ASSERT(sizeInBytes > 0);
			GADGET_ASSERT(sizeInBytes < std::numeric_limits<size_t>::max() / 8, "Bitset is too big, you won't be able to access all the bits");

			for(size_t i = 0; i < sizeInBytes; i++){
				bytes.Add(Byte(defaultValue_));
			}

			GADGET_BASIC_ASSERT(bytes.Capacity() == bytes.Size());
		}

		constexpr bool Get(size_t bitIndex_) const{
			size_t byteIndex = bitIndex_ / 8;
			size_t bit = bitIndex_ % 8;
			GADGET_BASIC_ASSERT(byteIndex < bytes.Size());

			const Byte& byte = bytes[byteIndex];
			switch(bit){
				case 0:
					return byte.bits.bit0;
				case 1:
					return byte.bits.bit1;
				case 2:
					return byte.bits.bit2;
				case 3:
					return byte.bits.bit3;
				case 4:
					return byte.bits.bit4;
				case 5:
					return byte.bits.bit5;
				case 6:
					return byte.bits.bit6;
				case 7:
					return byte.bits.bit7;
				default:
					GADGET_ASSERT(false, "Something went wrong in Bitset::Get");
					break;
			}

			return false;
		}

		constexpr void Set(size_t bitIndex_, bool value_){
			GADGET_BASIC_ASSERT((int)value_ == 0 || (int)value_ == 1);

			size_t byteIndex = bitIndex_ / 8;
			size_t bit = bitIndex_ % 8;
			GADGET_BASIC_ASSERT(byteIndex < bytes.Size());

			Byte& byte = bytes[byteIndex];
			switch(bit){
				case 0:
					byte.bits.bit0 = value_;
					break;
				case 1:
					byte.bits.bit1 = value_;
					break;
				case 2:
					byte.bits.bit2 = value_;
					break;
				case 3:
					byte.bits.bit3 = value_;
					break;
				case 4:
					byte.bits.bit4 = value_;
					break;
				case 5:
					byte.bits.bit5 = value_;
					break;
				case 6:
					byte.bits.bit6 = value_;
					break;
				case 7:
					byte.bits.bit7 = value_;
					break;
				default:
					GADGET_ASSERT(false, "Something went wrong in Bitset::Set");
					break;
			}
		}

	private:
		Array<Byte> bytes;
	};
}

#endif //!GADGET_BITSET_H