#ifndef HEAP_SCANNER_OBJARRAYKLASS_H
#define HEAP_SCANNER_OBJARRAYKLASS_H

#include "klass.h"

class ObjArrayKlass : public Klass {
private:
    uint64_t lengthOffset;
public:
    ObjArrayKlass(const char *addr, uint64_t lengthOffset, const std::shared_ptr<JvmTypesContainer> &typesContainer)
            : Klass(addr, typesContainer) {
        this->lengthOffset = lengthOffset;
    }

    int getObjectSize(char *addr) override;
};


#endif //HEAP_SCANNER_OBJARRAYKLASS_H
