#ifndef HEAP_SCANNER_JVMTYPESCONTAINER_H
#define HEAP_SCANNER_JVMTYPESCONTAINER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include "jvm.h"
#include "metadata/jvmMetadata.h"

class JvmTypesContainer {
private:
    std::unordered_map<std::string, std::shared_ptr<JvmMetadata>> typeMetadataMap;
    std::unordered_map<std::string, std::function<JvmMetadata *(std::shared_ptr<JVM>)>> metadataCreatorMap;
    std::shared_ptr<JVM> jvm;

public:
    JvmTypesContainer(std::shared_ptr<JVM> jvm);

    std::shared_ptr<JvmMetadata> getMetadata(const std::string& name);
};


#endif //HEAP_SCANNER_JVMTYPESCONTAINER_H
