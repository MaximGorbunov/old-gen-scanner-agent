#ifndef HEAP_SCANNER_G11HEAPMETADATA_H
#define HEAP_SCANNER_G11HEAPMETADATA_H

#include <memory>
#include <stdexcept>

#include "vmStructs.h"
#include "jvm.h"
#include "jvmMetadata.h"

class G1HeapMetadata : public JvmMetadata {
private:
    friend class G1Heap;

    std::shared_ptr<Field> hrmField;
public:
    explicit G1HeapMetadata(const std::shared_ptr<JVM> &jvm) {
        try {
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> g1CollectedHeapFields = jvm->getTypeFields(
                    "G1CollectedHeap");
            if (g1CollectedHeapFields == nullptr) {
                printf("Failed to find G1CollectedHeap fields!");
                throw std::out_of_range("Failed to find G1CollectedHeap fields!");
            }
            hrmField = g1CollectedHeapFields->at("_hrm");
        } catch (std::out_of_range &err) {
            printf("Failed to find _hrm field in G1CollectedHeap!");
            throw err;
        }
    }
};


#endif //HEAP_SCANNER_G11HEAPMETADATA_H
