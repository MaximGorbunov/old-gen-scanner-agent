#ifndef HEAP_SCANNER_SYMBOL_H
#define HEAP_SCANNER_SYMBOL_H

#include <iostream>

#include "jvmTypesContainer.h"
#include "jni.h"
#include "symbolMetadata.h"

class Symbol {
private:
    char* body;
    uint16_t length;
public:
    Symbol(const char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<SymbolMetadata>(typesContainer->getMetadata("Symbol"));
        if (metadata->length->isStatic) {
            length = *(uint16_t*) metadata->length->offset;
        } else {
            length = *(uint16_t*)(addr+metadata->length->offset);
        }
        if (metadata->body->isStatic) {
            body = (char*) metadata->body->offset;
        } else {
            body = (char*)(addr + metadata->body->offset);
        }
    }

    char* getBody();
};


#endif //HEAP_SCANNER_SYMBOL_H
