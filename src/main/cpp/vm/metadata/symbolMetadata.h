#ifndef HEAP_SCANNER_SYMBOLMETADATA_H
#define HEAP_SCANNER_SYMBOLMETADATA_H

#include "jvmMetadata.h"
#include "vmStructs.h"
#include "jvm.h"

class SymbolMetadata : public JvmMetadata {
private:
    friend class Symbol;

    std::shared_ptr<Field> length;
    std::shared_ptr<Field> body;
public:
    SymbolMetadata(const std::shared_ptr<JVM> &jvm) {
        auto klassFields = jvm->getTypeFields("Symbol");
        length = jvm->getFieldByName(klassFields, "_length");
        body = jvm->getFieldByName(klassFields, "_body");
    }
};

#endif //HEAP_SCANNER_SYMBOLMETADATA_H
