#include "g1Heap.h"
#include "jvm.h"
#include "oopDesc.h"

using namespace std;

void G1Heap::iterate(const std::shared_ptr<JVM> &jvm, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
    G1HeapRegionTable *pTable = hrm->getRegions();
    bool *compressedPointers = (bool *) jvm->getJVMFlagValue("UseCompressedClassPointers");
    if (compressedPointers == nullptr) {
        printf("Not found");
    }
    for (int i = 0; i < pTable->getLength(); i++) {
        HeapRegion *pRegion = pTable->getHeapRegionByIndex(i, typesContainer);
        if (pRegion == nullptr) break;
        auto bottom = pRegion->getBottom();
        while (bottom < pRegion->getTop()) {
            char *pOopDesc = (char *) bottom;
            printf("Bottom: %lX %lX", bottom, pRegion->getTop());
            auto oopDesc = new OopDesc(pOopDesc, typesContainer, compressedPointers);
            oopDesc->getKlass()->printName();
            int size = oopDesc->object_size();
            bottom += size;
        }
    }

}
