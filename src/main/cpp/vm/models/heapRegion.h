#ifndef HEAP_SCANNER_HEAPREGION_H
#define HEAP_SCANNER_HEAPREGION_H

#include "heapRegionType.h"
#include "vmStructs.h"
#include "vm/jvm.h"
#include "heapRegionMetadata.h"

class HeapRegion {
private:

    HeapRegionType *type;
    char *bottom;
    char *top;
    char *end;
    char *compactionTop;
public:
    explicit HeapRegion(const char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<HeapRegionMetadata>(typesContainer->getMetadata("HeapRegion"));

        if (metadata->typeField->isStatic) {
            type = new HeapRegionType((char *) metadata->typeField->offset, typesContainer);
        } else {
            type = new HeapRegionType((char *) (addr + metadata->typeField->offset), typesContainer);
        }
        if (metadata->bottomField->isStatic) {
            bottom = (char *) metadata->bottomField->offset;
        } else {
            bottom = *(char **) (addr + metadata->bottomField->offset);
        }
        if (metadata->topField->isStatic) {
            top = (char *) metadata->topField->offset;
        } else {
            top = *(char **) (addr + metadata->topField->offset);
        }
        if (metadata->endField->isStatic) {
            end = (char *) metadata->endField->offset;
        } else {
            end = *(char **) (addr + metadata->endField->offset);
        }
        if (metadata->compactionTopField->isStatic) {
            compactionTop = (char *) metadata->compactionTopField->offset;
        } else {
            compactionTop = *(char **) (addr + metadata->compactionTopField->offset);
        }
    }

    uintptr_t getBottom();

    uintptr_t getEnd();

    uintptr_t getTop();

    bool isHumongous();
    bool isContinueHumongous();
};


#endif //HEAP_SCANNER_HEAPREGION_H
