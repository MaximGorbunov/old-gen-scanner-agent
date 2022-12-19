#include "jvm.h"

#include <memory>
#include <stdexcept>
#include "vmStructs.h"

using namespace std;

void JVM::resolveVMStructs() {
    char *entry = gHotSpotVMStructs;
    for (;; entry += gHotSpotVMStructEntryArrayStride) {
        shared_ptr<Field> field(new Field());
        field->fieldName = *(char **) (entry + gHotSpotVMStructEntryFieldNameOffset);
        char *typeName = *(char **) (entry + gHotSpotVMStructEntryTypeNameOffset);
        if (typeName == nullptr || field->fieldName == nullptr) break;
        string typeNameStr = string(typeName);
        int32_t isStatic = *(int32_t *) (entry + gHotSpotVMStructEntryIsStaticOffset);
        uint64_t offset = *(uint64_t *) (entry + gHotSpotVMStructEntryOffsetOffset);
        uint64_t address = *(uint64_t *) (entry + gHotSpotVMStructEntryAddressOffset);
        field->typeString = *(char **) (entry + gHotSpotVMStructEntryTypeStringOffset);
        field->isStatic = isStatic;
        if (isStatic) {
            field->offset = address;
        } else {
            field->offset = offset;
        }
        auto search = JVM::vmStructs.find(typeNameStr);
        shared_ptr<unordered_map<string, shared_ptr<Field>>> fields;
        if (search == JVM::vmStructs.end()) {
            fields = std::make_shared<unordered_map<string, shared_ptr<Field>>>();
            JVM::vmStructs[typeNameStr] = fields;
        } else {
            fields = search->second;
        }
        fields->insert({string(field->fieldName), field});
    }
}

void JVM::resolveVMTypes() {
    char *entry = gHotSpotVMTypes;
    for (;; entry += gHotSpotVMTypeEntryArrayStride) {
        VMTypeEntry vmTypeEntry;
        vmTypeEntry.typeName = *(char **) (entry + gHotSpotVMTypeEntryTypeNameOffset);
        if (vmTypeEntry.typeName == nullptr) break;
        vmTypeEntry.superclassName = *(char **) (entry + gHotSpotVMTypeEntrySuperclassNameOffset);
        vmTypeEntry.isOopType = *(int32_t *) (entry + gHotSpotVMTypeEntryIsOopTypeOffset);
        vmTypeEntry.isIntegerType = *(int32_t *) (entry + gHotSpotVMTypeEntryIsIntegerTypeOffset);
        vmTypeEntry.isUnsigned = *(int32_t *) (entry + gHotSpotVMTypeEntryIsUnsignedOffset);
        vmTypeEntry.size = *(int64_t *) (entry + gHotSpotVMTypeEntrySizeOffset);
        vmTypeEntry.fields = JVM::vmStructs[vmTypeEntry.typeName];
        JVM::types[vmTypeEntry.typeName] = vmTypeEntry;
    }
}

void JVM::resolveIntConstants() {
    char *entry = gHotSpotVMIntConstants;
    for (;; entry += gHotSpotVMIntConstantEntryArrayStride) {
        char *constantNamePointer = *(char **) (entry + gHotSpotVMIntConstantEntryNameOffset);
        if (constantNamePointer == nullptr) break;
        auto constantName = new string(constantNamePointer);
        uint32_t constantValue = *(int *) (entry + gHotSpotVMIntConstantEntryValueOffset);
        JVM::intConstants[*constantName] = constantValue;
    }
}

void JVM::resolveLongConstants() {
    char *entry = gHotSpotVMLongConstants;
    for (;; entry += gHotSpotVMLongConstantEntryArrayStride) {
        char *constantNamePointer = *(char **) (entry + gHotSpotVMLongConstantEntryNameOffset);
        if (constantNamePointer == nullptr) break;
        auto constantName = new string(constantNamePointer);
        uint64_t constantValue = *(int *) (entry + gHotSpotVMLongConstantEntryValueOffset);
        JVM::longConstants[*constantName] = constantValue;
    }
}


shared_ptr<unordered_map<string, shared_ptr<Field>>> JVM::getTypeFields(const string &type) {
    return types[type].fields;
}

void *JVM::getG1CollectedHeap() {
    shared_ptr<unordered_map<string, shared_ptr<Field>>> fields = getTypeFields("Universe");
    shared_ptr<Field> _collectedHeapField;
    try {
        _collectedHeapField.swap(fields->at("_collectedHeap"));
    } catch (out_of_range &err) {
        printf("Could not find _collectedHeap field in Universe");
        throw err;
    }

    auto *pCollectedHeap = *(void **) _collectedHeapField->offset;
    bool isG1 = (uintptr_t)symbolsParser->isType("G1CollectedHeap", *(uintptr_t*)pCollectedHeap);
    if (!isG1) {
        printf("Not G1 GC!\n");
        return nullptr;
    }
    return pCollectedHeap;
}

uint32_t JVM::getIntConstant(const string &name) {
    return JVM::intConstants[name];
}

uint32_t JVM::getLongConstant(const string &name) {
    return JVM::longConstants[name];
}

shared_ptr<Field> JVM::getFieldByName(const shared_ptr<unordered_map<string, shared_ptr<Field>>> &typeFieldsMap,
                                      const string &fieldName) {
    auto field = typeFieldsMap->find(fieldName);
    if (field != typeFieldsMap->end()) {
        return field->second;
    } else {
        throw std::out_of_range("Field " + fieldName + " not found");
    }
}

void JVM::resolveJVMFlags() {
    auto jvmFlagType = types["JVMFlag"];
    auto jvmFlagFields = jvmFlagType.fields;
    auto flagsField = getFieldByName(jvmFlagFields, "flags");
    auto numFlagsField = getFieldByName(jvmFlagFields, "numFlags");
    auto nameField = getFieldByName(jvmFlagFields, "_name");
    auto addrField = getFieldByName(jvmFlagFields, "_addr");
    size_t numFlags = *(size_t *) numFlagsField->offset;
    char **flags = (char **) flagsField->offset;
    for (int i = 0; i < numFlags - 1; i++) {//last row contains null values
        string name = string(*(char **) (*flags + jvmFlagType.size * i + nameField->offset));
        char *addr = *(char **) (*flags + jvmFlagType.size * i + addrField->offset);
        jvmFlags[name] = addr;
    }
}

char *JVM::getJVMFlagValue(const std::string &name) {
    auto search = jvmFlags.find(name);
    if (search == jvmFlags.end()) return nullptr;
    return search->second;
}

uint64_t JVM::getTypeSize(const std::string &name) {
    auto typeIterator = types.find(name);
    if (typeIterator == types.end()) {
        throw std::runtime_error("Can't find type for name " + name);
    }
    return typeIterator->second.size;
}

shared_ptr<SymbolsParser> JVM::getSymbolParser() {
    return symbolsParser;
}