#ifndef HEAP_SCANNER_SYMBOL_H
#define HEAP_SCANNER_SYMBOL_H

#include <iostream>
#include <cstring>

#include "jvmTypesContainer.h"
#include "jni.h"
#include "symbolMetadata.h"

class Symbol {
private:
    std::string body;
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
            char copy[length + 1];
            copy[length] = 0;
            memcpy(copy, (char*) metadata->body->offset, length);
            body = std::string(copy);
        } else {
            char copy[length + 1];
            copy[length] = 0;
            memcpy(copy, (char*)(addr + metadata->body->offset), length);
            body = std::string(copy);
        }
    }

    std::string getBody();
    uint16_t getLength() const;
};


#endif //HEAP_SCANNER_SYMBOL_H
