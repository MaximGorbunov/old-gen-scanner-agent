#ifndef HEAP_SCANNER_HEAPREGIONMETADATA_H
#define HEAP_SCANNER_HEAPREGIONMETADATA_H

#include <memory>

#include "jvmMetadata.h"
#include "vmStructs.h"
#include "jvm.h"

class HeapRegionMetadata : public JvmMetadata {
private:
    friend class HeapRegion;

    std::shared_ptr<Field> typeField;
    std::shared_ptr<Field> bottomField;
    std::shared_ptr<Field> topField;
    std::shared_ptr<Field> endField;
    std::shared_ptr<Field> compactionTopField;
public:
    explicit HeapRegionMetadata(const std::shared_ptr<JVM> &jvm) {

        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> heapRegionFields = jvm->getTypeFields(
                "HeapRegion");
        if (heapRegionFields == nullptr) {
            printf("Failed to find HeapRegion fields!");
            throw std::out_of_range("Failed to find HeapRegion fields!");
        }
        typeField = jvm->getFieldByName(heapRegionFields, "_type");
        bottomField = jvm->getFieldByName(heapRegionFields, "_bottom");
        topField = jvm->getFieldByName(heapRegionFields, "_top");
        endField = jvm->getFieldByName(heapRegionFields, "_end");
        compactionTopField = jvm->getFieldByName(heapRegionFields, "_compaction_top");
    }
};


#endif //HEAP_SCANNER_HEAPREGIONMETADATA_H
