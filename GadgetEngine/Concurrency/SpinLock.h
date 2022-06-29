#ifndef GADGET_SPIN_LOCK_H
#define GADGET_SPIN_LOCK_H

#include <atomic>

namespace Gadget{
	class SpinLock{
	private:
		std::atomic_flag atomicFlag;

	public:
		SpinLock();

		void Acquire();
		bool TryAcquire();
		void Release();
	};
}

#endif // !GADGET_SPIN_LOCK_H