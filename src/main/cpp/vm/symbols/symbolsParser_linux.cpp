#ifdef __unix__

#include <link.h>
#include <cstring>
#include <vector>
#include "symbolsParser.h"
#include <climits>
#include <stdexcept>

#define JVM_LIB_NAME "libjvm"
#define VTBL_OFFSET 0x10 //We need to skip 16 bytes since object vtbl points to functions https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-components
#define SYMBOL_PREFIX "_ZTV"

using namespace std;

static int libCallback(struct dl_phdr_info *info, size_t size, void *data) {
    auto *map = (unordered_map<string, uintptr_t> *) data;
    if (strstr(info->dlpi_name, JVM_LIB_NAME)) {
        char *baseAddress = nullptr;
        for (int j = 0; j < info->dlpi_phnum; j++) {
            auto p_type = info->dlpi_phdr[j].p_type;
            if (p_type == PT_LOAD) {
                if (baseAddress == nullptr) {
                    baseAddress = (char *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr);
                    char *shstrtab = nullptr;
                    char *strtab = nullptr;
                    Elf64_Sym *symtab = nullptr;
                    size_t symbolsCount = 0;
                    auto *header = (Elf64_Ehdr *) baseAddress;
                    Elf64_Shdr headers[header->e_shnum];
                    auto fd = fopen(info->dlpi_name, "r");
                    fseek(fd, (long) header->e_shoff, SEEK_SET);
                    fread(&headers, sizeof(Elf64_Ehdr), header->e_shnum, fd);
                    Elf64_Shdr &x = headers[header->e_shstrndx];
                    shstrtab = (char *) malloc(sizeof(char) * x.sh_size);
                    fseek(fd, (long) x.sh_offset, SEEK_SET);
                    fread(shstrtab, 1, x.sh_size, fd);
                    for (int index = 0; index < header->e_shnum; index++) {
                        if (headers[index].sh_type == SHT_STRTAB &&
                            strcmp(".strtab", &shstrtab[headers[index].sh_name]) == 0) {
                            printf("Type %s", &shstrtab[headers[index].sh_name]);
                            strtab = (char *) malloc(sizeof(char) * headers[index].sh_size);
                            fseek(fd, (long) headers[index].sh_offset, SEEK_SET);
                            fread(strtab, 1, headers[index].sh_size, fd);
                        }
                        if (headers[index].sh_type == SHT_SYMTAB) {
                            printf("Addr loaded %p : %lX\n", &headers, headers[index].sh_offset);
                            symtab = (Elf64_Sym *) malloc(headers[index].sh_size);
                            symbolsCount = headers[index].sh_size / headers[index].sh_entsize;
                            fseek(fd, (long) headers[index].sh_offset, SEEK_SET);
                            fread(symtab, 1, headers[index].sh_size, fd);
                        }
                    }
                    for (int symIndex = 0; symIndex < symbolsCount; symIndex++) {
                        char *symName = &strtab[symtab[symIndex].st_name];
                        map->insert({string(symName), (uintptr_t )(symtab[symIndex].st_value + baseAddress)});
                    }
                    fclose(fd);
                    if (shstrtab != nullptr) {
                        free(shstrtab);
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

SymbolsParser::SymbolsParser() {
    this->typeVtblMap = make_shared<unordered_map<string, uintptr_t>>();
    dl_iterate_phdr(libCallback, this->typeVtblMap.get());
}

bool SymbolsParser::isType(const std::string &type, uintptr_t pointer) {
    auto mangledString = SYMBOL_PREFIX + to_string(type.size()) + type;
    auto vtblIterator = typeVtblMap->find(mangledString);
    if (vtblIterator == typeVtblMap->end()) {
        throw runtime_error("Cannot find vtbl for type " + mangledString);
    }
    return vtblIterator->second + VTBL_OFFSET  == pointer;
}


#endif