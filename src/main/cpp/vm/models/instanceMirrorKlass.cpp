#include "instanceMirrorKlass.h"
#include "globalDefinitions.h"

int InstanceMirrorKlass::getObjectSize(char *addr) {
    int i = *(int *) (addr + oopSizeOffset);
    return i << logHeapWordSize;
}