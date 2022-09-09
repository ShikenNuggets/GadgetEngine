#include "SpinLock.h"

#include <thread>

#include "Debug.h"

using namespace Gadget;

SpinLock::SpinLock() : atomicFlag(){}

void SpinLock::Acquire(){
	while(!TryAcquire()){
		std::this_thread::yield();
	}
}

bool SpinLock::TryAcquire(){
	return !(atomicFlag.test_and_set(std::memory_order_acquire));
}

void SpinLock::Release(){
	atomicFlag.clear(std::memory_order_release);
}

RaceConditionDetector::RaceConditionDetector() : flag(false){}

void RaceConditionDetector::Acquire(){
	GADGET_ASSERT(!flag, "Race condition detected (lock was re-acquired by another thread)!"); //No one should already have the lock
	flag = true;
}

void RaceConditionDetector::Release(){
	GADGET_ASSERT(flag, "Tried to release a lock that is not acquired. Possible race condition?"); //Release should only be called after Acquire
	flag = false;
}