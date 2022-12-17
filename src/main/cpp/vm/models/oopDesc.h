#ifndef HEAP_SCANNER_OOPDESC_H
#define HEAP_SCANNER_OOPDESC_H

#include <memory>

#include "vm/jvm.h"
#include "jvmTypesContainer.h"
#include "oopDescMetadata.h"
#include "instanceMirrorKlass.h"
#include "instanceKlass.h"

class OopDesc {
private:
    int mark;
    std::shared_ptr<Klass> klass;
    char *addr;
    bool compressedClassPointers;
    uint64_t klassGapOffset;
    uint64_t arrayTypeSize;
    int minObjAlignmentInBytes;
public:
    OopDesc(char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer, bool compressedClassPointers) {
        this->compressedClassPointers = compressedClassPointers;
        this->addr = addr;
        auto metadata = std::static_pointer_cast<OopDescMetadata>(typesContainer->getMetadata("OopDesc"));
        if (metadata->markField->isStatic) {
            mark = *(int *) metadata->markField->offset;
        } else {
            mark = *(int *) (addr + metadata->markField->offset);
        }
        uintptr_t pKlass;
        if (compressedClassPointers) {

            uintptr_t oopBase;
            if (metadata->oopBaseField->isStatic) {
                oopBase = *(std::uintptr_t *) metadata->oopBaseField->offset;
            } else {
                oopBase = *(std::uintptr_t *) (addr + metadata->oopBaseField->offset);
                printf("OopBase: %lu \n", oopBase);
            }

            uint32_t oopShift;
            if (metadata->oopShiftField->isStatic) {
                oopShift = *(std::uintptr_t *) metadata->oopShiftField->offset;
            } else {
                oopShift = *(std::uintptr_t *) (addr + metadata->oopShiftField->offset);
            }

            uint64_t compressedKlass;
            if (metadata->compressedKlassField->isStatic) {
                compressedKlass = *(uint32_t *) metadata->compressedKlassField->offset;
            } else {
                compressedKlass = *(uint32_t *) (addr + metadata->compressedKlassField->offset);
            }
            printf("oopBase: %lX", oopBase);
            printf("comp: %llX", compressedKlass);
            pKlass = oopBase + (long) (compressedKlass << oopShift);
        } else {
            if (metadata->klassField->isStatic) {
                pKlass = *(uintptr_t *) metadata->klassField->offset;
            } else {
                pKlass = *(uintptr_t *) addr + metadata->klassField->offset;
            }
        }
        printf("pointer: %lX %p", pKlass, addr);
        if (metadata->symbolsParser->isType("InstanceMirrorKlass", *(uintptr_t*)pKlass)) {
            klass = std::make_shared<InstanceMirrorKlass>((char *) pKlass, typesContainer);
        } else if (metadata->symbolsParser->isType("InstanceKlass", *(uintptr_t*)pKlass)) {
            klass = std::make_shared<InstanceKlass>((char *) pKlass, typesContainer);
        } else {
            klass = std::make_shared<Klass>((char *) pKlass, typesContainer);
        }
        arrayTypeSize = metadata->arrayTypeSize;
        klassGapOffset = metadata->klassGapOffset;
        minObjAlignmentInBytes = metadata->minObjAlignmentInBytes;
    }

    static inline int layout_helper_log2_element_size(int lh);

    static inline int layout_helper_header_size(int lh);

    int object_size();

    std::shared_ptr<Klass> getKlass();
};


#endif //HEAP_SCANNER_OOPDESC_H
