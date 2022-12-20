#ifndef HEAP_SCANNER_KLASS_H
#define HEAP_SCANNER_KLASS_H

#include <memory>

#include "cinttypes"
#include "jvmTypesContainer.h"
#include "klassMetadata.h"
#include "symbol.h"

class Klass {
private:
    std::shared_ptr<Symbol> name;
    int layoutHelper;
    char *klassAddr;
public:
    Klass(char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer) {
        auto metadata = std::static_pointer_cast<KlassMetadata>(typesContainer->getMetadata("Klass"));
        if (metadata->name->isStatic) {
            name = std::make_shared<Symbol>(*(char **) metadata->name->offset, typesContainer);
        } else {
            name = std::make_shared<Symbol>(*(char **) (addr + metadata->name->offset), typesContainer);
        }
        if (metadata->layoutHelper->isStatic) {
            layoutHelper = *(int *) metadata->layoutHelper->offset;
        } else {
            layoutHelper = *(int *) (addr + metadata->layoutHelper->offset);
        }
        klassAddr = addr;
    }

    virtual ~Klass() = default;

    virtual int getObjectSize(char *addr) {
        throw std::runtime_error("Get object size not implemented: ");
    };

    int getLayoutHelper();

    void printName();
};


#endif //HEAP_SCANNER_KLASS_H
