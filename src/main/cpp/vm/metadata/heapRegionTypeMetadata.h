#ifndef HEAP_SCANNER_HEAPREGIONTYPEMETADATA_H
#define HEAP_SCANNER_HEAPREGIONTYPEMETADATA_H

#include <cinttypes>
#include "jvm.h"
#include "jvmMetadata.h"

class HeapRegionTypeMetadata : public JvmMetadata {
private:
    friend class HeapRegionType;

    uint32_t freeTag;
    uint32_t youngMask;
    uint32_t edenTag;
    uint32_t survTag;
    uint32_t humongousMask;
    uint32_t pinnedMask;
    uint32_t archiveMask;
    uint32_t startsHumongousTag;
    uint32_t continuesHumongousTag;
    uint32_t oldMask;
    std::shared_ptr<Field> tagField;
public:
    explicit HeapRegionTypeMetadata(const std::shared_ptr<JVM> &jvm) {
        freeTag = jvm->getIntConstant("HeapRegionType::FreeTag");
        youngMask = jvm->getIntConstant("HeapRegionType::YoungMask");
        oldMask = jvm->getIntConstant("HeapRegionType::OldMask");
        edenTag = jvm->getIntConstant("HeapRegionType::EdenTag");
        survTag = jvm->getIntConstant("HeapRegionType::SurvTag");
        humongousMask = jvm->getIntConstant("HeapRegionType::HumongousMask");
        pinnedMask = jvm->getIntConstant("HeapRegionType::PinnedMask");
        archiveMask = jvm->getIntConstant("HeapRegionType::ArchiveMask");
        startsHumongousTag = jvm->getIntConstant("HeapRegionType::StartsHumongousTag");
        continuesHumongousTag = jvm->getIntConstant("HeapRegionType::ContinuesHumongousTag");

        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> fields;
        try {
            fields = jvm->getTypeFields("HeapRegionType");
        } catch (std::out_of_range &err) {
            printf("Cannot find fields of HeapRegionType!");
            throw err;
        }
        try {
            tagField = fields->at("_tag");
        } catch (std::out_of_range &err) {
            printf("Cannot find tag field of HeapRegionType!");
            throw err;
        }
    }
};

#endif //HEAP_SCANNER_HEAPREGIONTYPEMETADATA_H
