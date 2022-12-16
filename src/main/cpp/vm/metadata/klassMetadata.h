#ifndef HEAP_SCANNER_KLASSMETADATA_H
#define HEAP_SCANNER_KLASSMETADATA_H

#include <memory>

#include "jvmMetadata.h"
#include "jvm.h"

class KlassMetadata: public JvmMetadata {
private:
    friend class Klass;
    std::shared_ptr<Field> name;
    std::shared_ptr<Field> layoutHelper;
public:
    KlassMetadata(const std::shared_ptr<JVM> &jvm) {
        auto klassFields = jvm->getTypeFields("Klass");
        auto nameIterator = klassFields->find("_name");
        if (nameIterator == klassFields->end()) {
            throw std::runtime_error("Could not find _name field in Klass");
        } else {
            name = nameIterator->second;
        }
        auto layoutHelperIterator = klassFields->find("_layout_helper");
        if (layoutHelperIterator == klassFields->end()) {
            throw std::runtime_error("Could not find _layout_helper field in Klass");
        } else {
            layoutHelper = layoutHelperIterator->second;
        }
    }
};


#endif //HEAP_SCANNER_KLASSMETADATA_H
