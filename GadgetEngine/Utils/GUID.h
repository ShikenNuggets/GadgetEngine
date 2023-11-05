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

	public:
		static constexpr uint64_t Invalid = 0;

		GUID(uint64_t id_) : id(id_){}

		uint64_t Id() const{ return id; }

		inline constexpr bool operator==(const GUID& guid_) const{
			return id == guid_.id;
		}

		inline constexpr bool operator!=(const GUID& guid_) const{
			return id != guid_.id;
		}

		inline constexpr bool operator<(const GUID& guid_) const{
			return id < guid_.id;
		}

		inline constexpr bool operator<=(const GUID& guid_) const{
			return id <= guid_.id;
		}

		inline constexpr bool operator>(const GUID& guid_) const{
			return id > guid_.id;
		}

		inline constexpr bool operator >=(const GUID& guid_) const{
			return id >= guid_.id;
		}

		static GUID Generate(){
			GUID newId = GUID(nextUUID);
			nextUUID++;
			return newId;
		}
	};
}

#endif //!GADGET_UUID_H