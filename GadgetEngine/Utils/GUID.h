#ifndef GADGET_UUID_H
#define GADGET_UUID_H

#include <atomic>
#include <cstdint>

namespace Gadget{
	//This isn't strictly a *real* GUID implementation, but it's good enough for our purposes
	class GUID{
	private:
		static std::atomic<uint64_t> nextUUID;

		uint64_t id;

		GUID(uint64_t id_) : id(id_){}

	public:
		inline constexpr bool operator==(const GUID& uuid_){
			return id == uuid_.id;
		}

		inline constexpr bool operator!=(const GUID& uuid_){
			return id != uuid_.id;
		}

		static GUID Generate(){
			GUID newId = GUID(nextUUID);
			nextUUID++;
			return newId;
		}
	};
}

#endif //!GADGET_UUID_H