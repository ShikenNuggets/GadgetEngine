#include "GUID.h"

using namespace Gadget;

std::atomic<uint64_t> GUID::nextUUID = GUID::Invalid;