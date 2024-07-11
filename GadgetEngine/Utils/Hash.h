#ifndef GADGET_HASH_H
#define GADGET_HASH_H

namespace Gadget{
	namespace Hash{
		inline constexpr uint64_t GetBlock2(const char* string_, size_t len_){
			uint64_t as_int = 0;
			for(size_t i = 0; i < 8 && i < len_; i++){
				as_int = as_int * 256 + string_[i];
			}

			return as_int;
		}

		//MurmurHash64A - algorith created by Austin Appleby
		//Algorithm code is in the public domain: https://github.com/explosion/murmurhash/blob/master/murmurhash/MurmurHash2.cpp
		//More info here: https://en.wikipedia.org/wiki/MurmurHash
		inline constexpr uint64_t MurmurHash64A(const char* data_, size_t len_, uint64_t seed_ = 0){
			//Some assumptions made by the code here
			//We can read a 4-byte value from any address without crashing (no idea how to check this, TODO? Need to modify the algorithm if aligned reads are required)
			//Will not produce the same results on machines with different endianness (do we care? TODO? Endian-neutral version is significantly slower)

			const uint64_t m = 0xc6a4a7935bd1e995;
			const int32_t r = 47;

			uint64_t h = seed_ ^ (len_ * m);

			const char* data = data_;
			const char* end = data_ + (len_);

			while(data < end){
				uint64_t k = GetBlock2(data, len_);

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;

				data += sizeof(uint64_t);
			}

			switch(len_ & 7){
				case 7: h ^= uint64_t(data_[6]) << 48; [[fallthrough]];
				case 6: h ^= uint64_t(data_[5]) << 40; [[fallthrough]];
				case 5: h ^= uint64_t(data_[4]) << 32; [[fallthrough]];
				case 4: h ^= uint64_t(data_[3]) << 24; [[fallthrough]];
				case 3: h ^= uint64_t(data_[2]) << 16; [[fallthrough]];
				case 2: h ^= uint64_t(data_[1]) << 8; [[fallthrough]];
				case 1: h ^= uint64_t(data_[0]);
					h *= m;
			};

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}
	};
}

#endif //!GADGET_HASH_H