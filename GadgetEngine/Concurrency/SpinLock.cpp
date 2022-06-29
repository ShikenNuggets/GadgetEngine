#include "SpinLock.h"

#include <thread>

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