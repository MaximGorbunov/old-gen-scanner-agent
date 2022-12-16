#include <unordered_map>
#include "heapRegionManager.h"

using namespace std;

G1HeapRegionTable *HeapRegionManager::getRegions() {
    return regions;
}