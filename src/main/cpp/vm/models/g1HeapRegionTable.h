#ifndef HEAP_SCANNER_G1HEAPREGIONTABLE_H
#define HEAP_SCANNER_G1HEAPREGIONTABLE_H

#include <cstddef>
#include "heapRegion.h"
#include "vm/jvmTypesContainer.h"
#include "g1HeapRegionTableMetadata.h"

class G1HeapRegionTable {
private:
    char **base;
    size_t length;
    char **biasedBase;
    size_t bias;
    unsigned int shiftBy;
public:
    G1HeapRegionTable(const char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<G1HeapRegionTableMetadata>(
                typesContainer->getMetadata("G1HeapRegionTable"));
        if (metadata->baseField->isStatic) {
            base = *(char ***) metadata->baseField->offset;
        } else {
            base = *(char ***) (addr + metadata->baseField->offset);
        }
        if (metadata->lengthField->isStatic) {
            length = *(size_t *) metadata->lengthField->offset;
        } else {
            length = *(size_t *) (addr + metadata->lengthField->offset);
        }
        if (metadata->biasedBaseField->isStatic) {
            biasedBase = *(char ***) metadata->biasedBaseField->offset;
        } else {
            biasedBase = *(char ***) (addr + metadata->biasedBaseField->offset);
        }
        if (metadata->biasField->isStatic) {
            bias = *(size_t *) metadata->biasField->offset;
        } else {
            bias = *(size_t *) (addr + metadata->biasField->offset);
        }
        if (metadata->shiftByField->isStatic) {
            shiftBy = *(unsigned int *) metadata->shiftByField->offset;
        } else {
            shiftBy = *(unsigned int *) (addr + metadata->shiftByField->offset);
        }
    }

    HeapRegion *getHeapRegionByAddress(const char *addr, const std::shared_ptr<JvmTypesContainer>& typesContainer);

    HeapRegion *getHeapRegionByIndex(size_t index, const std::shared_ptr<JvmTypesContainer>& typesContainer);

    void verifyBiasedIndex(size_t index);

    size_t getLength() const;
};


#endif //HEAP_SCANNER_G1HEAPREGIONTABLE_H
