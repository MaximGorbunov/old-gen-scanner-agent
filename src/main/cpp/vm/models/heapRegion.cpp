#include <unordered_map>
#include "heapRegion.h"

using namespace std;

uintptr_t HeapRegion::getBottom() { return (uintptr_t) bottom; }

uintptr_t HeapRegion::getEnd() { return (uintptr_t) end; }

uintptr_t HeapRegion::getTop() { return (uintptr_t) top; }

bool HeapRegion::isHumongous() { return type->isHumongous(); }

bool HeapRegion::isContinueHumongous() { return type->isContinueHumongous(); }