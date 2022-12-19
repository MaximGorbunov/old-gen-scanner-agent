#ifndef HEAP_SCANNER_HEAPREGIONMANAGERMETADATA_H
#define HEAP_SCANNER_HEAPREGIONMANAGERMETADATA_H

#include <memory>
#include <stdexcept>

#include "vmStructs.h"
#include "jvmMetadata.h"
#include "jvm.h"

class HeapRegionManagerMetadata : public JvmMetadata {
private:
    friend class HeapRegionManager;

    std::shared_ptr<Field> regionsField;
public:
    explicit HeapRegionManagerMetadata(const std::shared_ptr<JVM> &jvm) {
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> heapRegionManagerFields;
        try {
            heapRegionManagerFields = jvm->getTypeFields("HeapRegionManager");
            if (heapRegionManagerFields == nullptr) {
                printf("Failed to find HeapRegionManager fields!");
                throw std::out_of_range("Failed to find HeapRegionManager fields!");
            }
            regionsField = heapRegionManagerFields->at("_regions");
        } catch (std::out_of_range &err) {
            printf("Failed to find _regions field in HeapRegionManager!");
            throw err;
        }
    }
};


#endif //HEAP_SCANNER_HEAPREGIONMANAGERMETADATA_H
