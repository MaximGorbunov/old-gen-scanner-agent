#ifndef HEAP_SCANNER_SYMBOLSPARSER_H
#define HEAP_SCANNER_SYMBOLSPARSER_H

#include <unordered_map>
#include <memory>
#include <string>

class SymbolsParser {
private:
    std::shared_ptr<std::unordered_map<std::string, uintptr_t>> typeVtblMap;
public:
    SymbolsParser();

    bool isType(const std::string &type, uintptr_t pointer);
};

#endif //HEAP_SCANNER_SYMBOLSPARSER_H
