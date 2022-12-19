#ifdef __APPLE__

#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

#include "symbolsParser.h"

#define LINKEDIT_SEGMENT "__LINKEDIT"
#define TEXT_SEGMENT "__TEXT"
#define JVM_LIB_NAME "libjvm"
#define VIRTUAL_FUNCTION_POINTERS_OFFSET 0x10
#define TYPE_PREFIX  "__ZTV"

using namespace std;

SymbolsParser::SymbolsParser() {
    this->typeVtblMap = make_shared<unordered_map<string, uintptr_t>>();
    uint32_t imagesCount = _dyld_image_count();
    for (int imageIndex = 0; imageIndex < imagesCount; imageIndex++) {
        const auto imageName = std::string(_dyld_get_image_name(imageIndex));
        if (imageName.find(JVM_LIB_NAME) != std::string::npos) {
            const mach_header_64 *pHeader = (mach_header_64 *) _dyld_get_image_header(imageIndex);
            const ::intptr_t slide = _dyld_get_image_vmaddr_slide(imageIndex);
            uint32_t loadCommandCount = pHeader->ncmds;
            auto *loadCommand = (load_command *) (pHeader + 1);
            char *linkeditSegment;
            char *textSegment;
            for (int j = 0; j < loadCommandCount; j++) {
                if (loadCommand->cmd == LC_SEGMENT_64) {
                    auto com = (segment_command_64 *) loadCommand;
                    if (strcmp(LINKEDIT_SEGMENT, com->segname) == 0) {
                        linkeditSegment = (char *) (com->vmaddr + slide - com->fileoff);
                    }
                    if (strcmp(TEXT_SEGMENT, com->segname) == 0) {
                        textSegment = (char *) (com->vmaddr + slide - com->fileoff);
                    }
                }
                if (loadCommand->cmd == LC_SYMTAB) {
                    auto *com = (symtab_command *) loadCommand;
                    auto nlist = (nlist_64 *) (linkeditSegment + com->symoff);
                    auto strTable = (char *) linkeditSegment + com->stroff;
                    for (int sym = 0; sym < com->nsyms; sym++) {
                        nlist_64 &nlist64 = nlist[sym];
                        if ((strTable + nlist64.n_un.n_strx) != nullptr && nlist64.n_value != 0) {
                            typeVtblMap->insert({string((char *) (strTable + nlist64.n_un.n_strx)),
                                                 (uintptr_t) textSegment + nlist64.n_value});
                        }
                    }
                }
                loadCommand = (load_command *) ((char *) loadCommand + loadCommand->cmdsize);
            }
        }
    }
}


bool SymbolsParser::isType(const std::string &type, uintptr_t pointer)  {
    std::string mangledType = TYPE_PREFIX + to_string(type.size()) + type;
    auto vtblIterator = typeVtblMap->find(mangledType);
    uintptr_t vtbl;
    if (vtblIterator == typeVtblMap->end()) {
        vtbl = 0;
    } else {
        vtbl = vtblIterator->second;
    }
    return pointer == (vtbl  + VIRTUAL_FUNCTION_POINTERS_OFFSET);
}

#endif
