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
            length = *(uint16_t *) metadata->length->offset;
        } else {
            length = *(uint16_t *) (addr + metadata->length->offset);
        }
        if (metadata->body->isStatic) {
            body = std::string((char *) metadata->body->offset, (char *) metadata->body->offset + length);
        } else {
            body = std::string((char *) metadata->body->offset, (char *) (addr + metadata->body->offset + length));
        }
    }

    std::string getBody();

    uint16_t getLength() const;
};


#endif //HEAP_SCANNER_SYMBOL_H
