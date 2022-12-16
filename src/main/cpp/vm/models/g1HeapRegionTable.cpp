#include <unordered_map>
#include <string>

#include "heapRegion.h"
#include "g1HeapRegionTable.h"
#include "vm/jvm.h"

using namespace std;

HeapRegion *G1HeapRegionTable::getHeapRegionByAddress(const char *addr, const std::shared_ptr<JvmTypesContainer>& typesContainer) {
    size_t biasedIndex = (uintptr_t) addr >> shiftBy;
    verifyBiasedIndex(biasedIndex);
    return new HeapRegion((char *) &biasedBase[biasedIndex], typesContainer);
}

HeapRegion *G1HeapRegionTable::getHeapRegionByIndex(size_t index, const std::shared_ptr<JvmTypesContainer>& typesContainer) {
    char *addr = base[index];
    if (addr == nullptr) {
        return nullptr;
    }
    return new HeapRegion(addr, typesContainer);
}

void G1HeapRegionTable::verifyBiasedIndex(size_t index) {
    if (biasedBase == nullptr) {
        throw runtime_error("Biased base is null pointer!");
    }
    if (!(index >= bias && index <= (bias + length))) {
        throw runtime_error("Biased index out of inclusive bounds, index:" + to_string(index));
    }
}

size_t G1HeapRegionTable::getLength() const { return length; }