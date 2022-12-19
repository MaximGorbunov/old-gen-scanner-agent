#include "heapRegionType.h"
#include "vm/jvm.h"
#include "heapRegionTypeMetadata.h"

using namespace std;

bool HeapRegionType::isOld() {
    return this->tag & metadata->oldMask;
}
bool HeapRegionType::isHumongous() {
    return (this->tag & metadata->humongousMask) > 0;
}

bool HeapRegionType::isContinueHumongous() {
    return (this->tag & metadata->continuesHumongousTag) > 0;
}