#ifndef HEAP_SCANNER_G1COLLECTEDHEAP_H
#define HEAP_SCANNER_G1COLLECTEDHEAP_H

#include "heapRegionManager.h"
#include "g1HeapMetadata.h"

class G1Heap {
private:
    HeapRegionManager *hrm;
public:
    G1Heap(char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<G1HeapMetadata>(typesContainer->getMetadata("G1Heap"));
        if (metadata->hrmField->isStatic) {
            this->hrm = new HeapRegionManager((char *) metadata->hrmField->offset, typesContainer);
        } else {
            this->hrm = new HeapRegionManager((char *) (addr + metadata->hrmField->offset), typesContainer);
        }
    }

    void iterate(const std::shared_ptr<JVM> &jvm, const std::shared_ptr<JvmTypesContainer> &typesContainer);
};


#endif //HEAP_SCANNER_G1COLLECTEDHEAP_H
