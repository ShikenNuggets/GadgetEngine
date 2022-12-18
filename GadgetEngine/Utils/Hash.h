#ifndef GADGET_HASH_H
#define GADGET_HASH_H

#include <string>

namespace Gadget{
	class Hash{
	public:
		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		static inline constexpr uint32_t CRC32(const char* data_, size_t len_){
			uint32_t crc = 0;

			for(size_t i = 0; i < len_; i++){
				crc = crc32_for_byte(crc ^ (data_)[i]) ^ crc >> 8;
			}

			return crc;
		}

		//MurmurHash64A - algorith created by Austin Appleby
		//Algorithm code is in the public domain: https://github.com/explosion/murmurhash/blob/master/murmurhash/MurmurHash2.cpp
		//More info here: https://en.wikipedia.org/wiki/MurmurHash
		static inline constexpr uint64_t MurmurHash64A(const char* data_, size_t len_, uint64_t seed_ = 0){
			//Some assumptions made by the code here
			static_assert(sizeof(int) == 4);
			static_assert(sizeof(long long) == 8);
			//We can read a 4-byte value from any address without crashing (no idea how to check this, TODO? Need to modify the algorithm if aligned reads are required)
			//Will not produce the same results on machines with different endianness (do we care? TODO? Endian-neutral version is significantly slower)

			const uint64_t m = 0xc6a4a7935bd1e995;
			const int r = 47;

			uint64_t h = seed_ ^ (len_ * m);

			const uint64_t* data = (const uint64_t*)data_; //TODO - Apparently this cast is not constexpr friendly
			const uint64_t* end = data + (len_ / 8);

			while(data != end){
				uint64_t k = GetBlock(data++);

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char* data2 = (const unsigned char*)data;

			switch(len_ & 7){
				case 7: h ^= uint64_t(data2[6]) << 48; [[fallthrough]];
				case 6: h ^= uint64_t(data2[5]) << 40; [[fallthrough]];
				case 5: h ^= uint64_t(data2[4]) << 32; [[fallthrough]];
				case 4: h ^= uint64_t(data2[3]) << 24; [[fallthrough]];
				case 3: h ^= uint64_t(data2[2]) << 16; [[fallthrough]];
				case 2: h ^= uint64_t(data2[1]) << 8; [[fallthrough]];
				case 1: h ^= uint64_t(data2[0]);
					h *= m;
			};

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}

	private:
		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		static inline constexpr uint32_t crc32_for_byte(uint32_t r){
			for(int i = 0; i < 8; i++){
				r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
			}

			return r ^ (uint32_t)0xFF000000L;
		}

		static inline constexpr uint64_t GetBlock(const uint64_t* p_){
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

		//Delete unwanted compiler-generated constructors, destructors, and assignment operators
		Hash() = delete;
		Hash(const Hash&) = delete;
		Hash(Hash&&) = delete;
		Hash& operator=(const Hash&) = delete;
		Hash& operator=(Hash&&) = delete;
		~Hash() = delete;
	};
}

#endif //!GADGET_HASH_H