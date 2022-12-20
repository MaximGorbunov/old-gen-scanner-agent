#ifndef HEAP_SCANNER_INSTANCEKLASS_H
#define HEAP_SCANNER_INSTANCEKLASS_H

#include "klass.h"

class InstanceKlass : public Klass {
public:
    InstanceKlass(char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer)
            : Klass(addr, typesContainer) {
    }

    ~InstanceKlass() override = default;

    int getObjectSize(char *addr) override;
};


#endif //HEAP_SCANNER_INSTANCEKLASS_H
