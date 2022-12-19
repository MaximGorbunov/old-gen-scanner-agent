#ifndef HEAP_SCANNER_G1HEAPREGIONTABLEMETADATA_H
#define HEAP_SCANNER_G1HEAPREGIONTABLEMETADATA_H

#include <stdexcept>
#include "jvm.h"
#include "jvmMetadata.h"
#include "vmStructs.h"

class G1HeapRegionTableMetadata : public JvmMetadata {
private:
    friend class G1HeapRegionTable;

    std::shared_ptr<Field> baseField;
    std::shared_ptr<Field> lengthField;
    std::shared_ptr<Field> biasedBaseField;
    std::shared_ptr<Field> biasField;
    std::shared_ptr<Field> shiftByField;
public:
    explicit G1HeapRegionTableMetadata(const std::shared_ptr<JVM> &jvm) {
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> g1HeapRegionTableFields;
        try {
            g1HeapRegionTableFields = jvm->getTypeFields("G1HeapRegionTable");
        } catch (std::out_of_range &err) {
            printf("Failed to find G1HeapRegionTable fields!");
            throw std::out_of_range("Failed to find G1HeapRegionTable fields!");
        }

        baseField = jvm->getFieldByName(g1HeapRegionTableFields, "_base");
        lengthField = jvm->getFieldByName(g1HeapRegionTableFields, "_length");
        biasedBaseField = jvm->getFieldByName(g1HeapRegionTableFields, "_biased_base");
        biasField = jvm->getFieldByName(g1HeapRegionTableFields, "_bias");
        shiftByField = jvm->getFieldByName(g1HeapRegionTableFields, "_shift_by");
    }
};


#endif //HEAP_SCANNER_G1HEAPREGIONTABLEMETADATA_H
