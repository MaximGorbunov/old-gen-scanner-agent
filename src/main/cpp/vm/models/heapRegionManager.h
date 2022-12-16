#ifndef HEAP_SCANNER_HEAPREGIONMANAGER_H
#define HEAP_SCANNER_HEAPREGIONMANAGER_H

#include "g1HeapRegionTable.h"
#include "vmStructs.h"
#include "vm/jvm.h"
#include "heapRegionManagerMetadata.h"

class HeapRegionManager {
private:
    G1HeapRegionTable *regions;
public:
    explicit HeapRegionManager(const char *addr, const std::shared_ptr<JvmTypesContainer>& typesContainer) {
        auto metadata = std::static_pointer_cast<HeapRegionManagerMetadata>(typesContainer->getMetadata("HeapRegionManager"));
        if (metadata->regionsField->isStatic) {
            regions = new G1HeapRegionTable((char *) metadata->regionsField->offset, typesContainer);
        } else {
            regions = new G1HeapRegionTable((char*)(addr + metadata->regionsField->offset), typesContainer);
        }
    }

    G1HeapRegionTable *getRegions();
};


#endif //HEAP_SCANNER_HEAPREGIONMANAGER_H
