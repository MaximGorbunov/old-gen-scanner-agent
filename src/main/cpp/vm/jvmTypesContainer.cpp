#include "jvmTypesContainer.h"

#include <utility>
#include "g1HeapMetadata.h"
#include "g1HeapRegionTableMetadata.h"
#include "heapRegionManagerMetadata.h"
#include "heapRegionTypeMetadata.h"
#include "heapRegionMetadata.h"
#include "oopDescMetadata.h"
#include "klassMetadata.h"
#include "symbolMetadata.h"
#include "javaLangClassMetadata.h"

JvmTypesContainer::JvmTypesContainer(std::shared_ptr<JVM> jvm) {
    this->jvm = std::move(jvm);
    typeMetadataMap = std::unordered_map<std::string, std::shared_ptr<JvmMetadata>>();
    metadataCreatorMap = std::unordered_map<std::string, std::function<JvmMetadata *(std::shared_ptr<JVM>)>>();
    metadataCreatorMap["G1Heap"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new G1HeapMetadata(jvm);
    };
    metadataCreatorMap["G1HeapRegionTable"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new G1HeapRegionTableMetadata(jvm);
    };
    metadataCreatorMap["HeapRegionManager"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new HeapRegionManagerMetadata(jvm);
    };
    metadataCreatorMap["HeapRegion"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new HeapRegionMetadata(jvm);
    };
    metadataCreatorMap["HeapRegionType"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new HeapRegionTypeMetadata(jvm);
    };
    metadataCreatorMap["Klass"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new KlassMetadata(jvm);
    };
    metadataCreatorMap["OopDesc"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new OopDescMetadata(jvm);
    };
    metadataCreatorMap["Symbol"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new SymbolMetadata(jvm);
    };
    metadataCreatorMap["JavaLangClass"] = [](const std::shared_ptr<JVM> &jvm) -> JvmMetadata * {
        return new JavaLangClassMetadata(jvm);
    };
}

std::shared_ptr<JvmMetadata> JvmTypesContainer::getMetadata(const std::string &name) {
    auto search = typeMetadataMap.find(name);
    std::shared_ptr<JvmMetadata> metadata;
    if (search == typeMetadataMap.end()) {
        metadata = std::shared_ptr<JvmMetadata>(metadataCreatorMap[name](jvm));
        typeMetadataMap.insert({name, metadata});
    } else {
        metadata = search->second;
    }
    return metadata;
}