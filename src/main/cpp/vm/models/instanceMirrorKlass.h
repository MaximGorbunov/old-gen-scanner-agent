#ifndef HEAP_SCANNER_INSTANCEMIRRORKLASS_H
#define HEAP_SCANNER_INSTANCEMIRRORKLASS_H

#include "instanceKlass.h"
#include "javaLangClassMetadata.h"

class InstanceMirrorKlass : public InstanceKlass {
private:
    int oopSizeOffset;
public:
    InstanceMirrorKlass(char *addr, const std::shared_ptr<JvmTypesContainer> &typesContainer)
            : InstanceKlass(addr, typesContainer) {
        auto classMetadata = std::static_pointer_cast<JavaLangClassMetadata>(
                typesContainer->getMetadata("JavaLangClass"));

        oopSizeOffset = *(int *) (classMetadata->oopSizeOffset->offset);
    }

    int getObjectSize(char *addr) override;
};

#endif //HEAP_SCANNER_INSTANCEMIRRORKLASS_H
