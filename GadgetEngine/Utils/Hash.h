#ifndef GADGET_HASH_H
#define GADGET_HASH_H

#include <string>

namespace Gadget{
	class Hash{
	public:
		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		static inline constexpr uint32_t CRC32(const void* data_, size_t len_){
			uint32_t crc = 0;

			for(size_t i = 0; i < len_; i++){
				crc = crc32_for_byte((uint8_t)crc ^ ((const uint8_t*)data_)[i]) ^ crc >> 8;
			}

			return crc;
		}

		//TODO - We need a 64 bit hashing function (CRC64? MD5? idk)

	private:
		//Shamelessly copied from SDL2 (SDL_crc32.c)
		//Pasted here so it can be constexpr
		static inline constexpr uint32_t crc32_for_byte(uint32_t r){
			for(int i = 0; i < 8; i++){
				r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
			}

			return r ^ (uint32_t)0xFF000000L;
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