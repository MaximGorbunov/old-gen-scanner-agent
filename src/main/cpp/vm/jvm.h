#ifndef HEAP_SCANNER_JVM_H
#define HEAP_SCANNER_JVM_H

#include <string>
#include <list>
#include <unordered_map>

#include "vmStructs.h"
#include "jdkHeap.h"

class JVM {
private:
    void resolveVMStructs();
    void resolveVMTypes();
    void resolveIntConstants();
    void resolveLongConstants();
    void resolveJVMFlags();
public:
    JVM() {
        resolveVMStructs();
        resolveVMTypes();
        resolveIntConstants();
        resolveLongConstants();
        resolveJVMFlags();
    }
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> getTypeFields(const std::string &type);
    CollectedHeap *getG1CollectedHeap();
    uint32_t getIntConstant(const std::string &name);
    uint32_t getLongConstant(const std::string &name);
    std::shared_ptr<Field> getFieldByName(const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>> &typeFieldsMap, const std::string &fieldName);

    char* getJVMFlagValue(const std::string &name);
    uint64_t getTypeSize(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Field>>>> vmStructs;
    std::unordered_map<std::string, VMTypeEntry> types;
    std::unordered_map<std::string, uint32_t> intConstants;
    std::unordered_map<std::string, uint64_t> longConstants;
    std::unordered_map<std::string, char*> jvmFlags;
};


#endif //HEAP_SCANNER_JVM_H
