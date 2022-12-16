#include "heapRegionType.h"
#include "vm/jvm.h"
#include "heapRegionTypeMetadata.h"

using namespace std;

bool HeapRegionType::isOld() {
//    return this->tag & metadata->oldMask || this->tag & metadata->humongousMask;
    return false;
}