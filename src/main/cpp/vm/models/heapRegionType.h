#ifndef HEAP_SCANNER_HEAPREGIONTYPE_H
#define HEAP_SCANNER_HEAPREGIONTYPE_H

#include <cinttypes>
#include <string>
#include <unordered_map>

#include "vmStructs.h"
#include "vm/jvm.h"
#include "jvmTypesContainer.h"
#include "heapRegionTypeMetadata.h"

class HeapRegionType {
private:
    uint32_t tag;
//    std::shared_ptr<HeapRegionTypeMetadata> metadata;
public:

    HeapRegionType(const char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<HeapRegionTypeMetadata>(typesContainer->getMetadata("HeapRegionType"));
        if (metadata->tagField->isStatic) {
            tag = *(uint32_t *) metadata->tagField->offset;
        } else {
            tag = *(uint32_t *) addr + metadata->tagField->offset;
        }
    }

    bool isOld();
};


#endif //HEAP_SCANNER_HEAPREGIONTYPE_H
