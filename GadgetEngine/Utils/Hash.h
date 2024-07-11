#ifndef GADGET_HASH_H
#define GADGET_HASH_H

namespace Gadget{
	namespace Hash{
		//Function prototypes
		inline constexpr uint32_t CRC32(const char* data_, size_t len_);
		inline constexpr uint64_t MurmurHash64A(const char* data_, size_t len_, uint64_t seed_);
		inline constexpr uint32_t crc32_for_byte(uint32_t r);
		inline constexpr uint64_t GetBlock(const uint64_t* p_);
		constexpr uint64_t GetBlock2(std::string_view string_, size_t len_);

		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		inline constexpr uint32_t CRC32(const char* data_, size_t len_){
			uint32_t crc = 0;

			for(size_t i = 0; i < len_; i++){
				crc = crc32_for_byte(crc ^ (data_)[i]) ^ crc >> 8;
			}

			return crc;
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

		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		inline constexpr uint32_t crc32_for_byte(uint32_t r){
			for(int i = 0; i < 8; i++){
				r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
			}

			return r ^ (uint32_t)0xFF000000L;
		}

		inline constexpr uint64_t GetBlock2(std::string_view string_, size_t len_){
			uint64_t as_int = 0;
			for(size_t i = 0; i < 8 && i < len_; i++){
				as_int = as_int * 256 + string_[i];
			}

			return as_int;
		}

		inline constexpr uint64_t GetBlock(const uint64_t* p_){
			//TODO - There's an optimization we can do here if we know the machine is little Endian
			const uint8_t* c = (const uint8_t*)p_;
			return (uint64_t)c[0] |
				(uint64_t)c[1] << 8 |
				(uint64_t)c[2] << 16 |
				(uint64_t)c[3] << 24 |
				(uint64_t)c[4] << 32 |
				(uint64_t)c[5] << 40 |
				(uint64_t)c[6] << 48 |
				(uint64_t)c[7] << 56;
		}
	};
}

#endif //!GADGET_HASH_H