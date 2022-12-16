#ifndef HEAP_SCANNER_OOPDESCMETADATA_H
#define HEAP_SCANNER_OOPDESCMETADATA_H

#include "jvmMetadata.h"
#include "vmStructs.h"
#include "jvm.h"

class OopDescMetadata : public JvmMetadata {
private:
    friend class OopDesc;

    std::shared_ptr<Field> markField;
    std::shared_ptr<Field> oopBaseField;
    std::shared_ptr<Field> oopShiftField;
    std::shared_ptr<Field> compressedKlassField;
    std::shared_ptr<Field> klassField;
    uint64_t arrayTypeSize;
    uint64_t klassGapOffset;
    int minObjAlignmentInBytes;
public:
    explicit OopDescMetadata(const std::shared_ptr<JVM> &jvm) {
        auto compressedOops = jvm->getTypeFields("CompressedKlassPointers");
        oopBaseField = jvm->getFieldByName(compressedOops, "_narrow_klass._base");
        oopShiftField = jvm->getFieldByName(compressedOops, "_narrow_klass._shift");

        auto oopDesc = jvm->getTypeFields("oopDesc");
        arrayTypeSize = (int) jvm->getTypeSize("arrayOopDesc");
        markField = jvm->getFieldByName(oopDesc, "_mark");
        compressedKlassField = jvm->getFieldByName(oopDesc, "_metadata._compressed_klass");
        klassField = jvm->getFieldByName(oopDesc, "_metadata._klass");
        klassGapOffset = (int) (klassField->offset + jvm->getTypeSize("narrowKlass"));
        minObjAlignmentInBytes = *(int *) jvm->getJVMFlagValue("ObjectAlignmentInBytes");
    }
};


#endif //HEAP_SCANNER_OOPDESCMETADATA_H
