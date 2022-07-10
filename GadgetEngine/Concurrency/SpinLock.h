#ifndef GADGET_SPIN_LOCK_H
#define GADGET_SPIN_LOCK_H

#include <atomic>

namespace Gadget{
	class SpinLock{
	public:
		SpinLock();

		void Acquire();
		bool TryAcquire();
		void Release();
	
	private:
		std::atomic_flag atomicFlag;

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		SpinLock(const SpinLock&) = delete;
		SpinLock(SpinLock&&) = delete;
		SpinLock& operator=(const SpinLock&) = delete;
		SpinLock& operator=(SpinLock&&) = delete;
	};

	//Lightweight spinlock used specifically for detecting race conditions
	//Use this to confirm if you need an actual lock or not
	//Use the macros defined below so they aren't included in Release builds
	class RaceConditionDetector{
	public:
		RaceConditionDetector();

		void Acquire();
		void Release();

	private:
		volatile bool flag; //Does not need to be atomic since this is just to detect races and not prevent them. Volatile so it doesn't get optimized out

		//Delete unwanted compiler-generated copy/move constructors and assignment operators
		RaceConditionDetector(const RaceConditionDetector&) = delete;
		RaceConditionDetector(RaceConditionDetector&&) = delete;
		RaceConditionDetector& operator=(const RaceConditionDetector&) = delete;
		RaceConditionDetector& operator=(RaceConditionDetector&&) = delete;
	};

#ifdef GADGET_DEBUG
	#define BEGIN_RACE_CONDITION_DETECTION(L) (L).Acquire();
	#define END_RACE_CONDITION_DETECTION(L) (L).Release();
#else
	#define BEGIN_RACE_CONDITION_DETECTION(L)
	#define END_RACE_CONDITION_DETECTION(L)
#endif //GADGET_DEBUG

}

#endif // !GADGET_SPIN_LOCK_H