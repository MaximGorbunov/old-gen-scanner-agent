#include "instanceKlass.h"
#include "globalDefinitions.h"

int InstanceKlass::getObjectSize(char *addr) {
    int layoutHelper = getLayoutHelper();
    if (layoutHelper <= LH_NEUTRAL) {
        throw std::runtime_error("Must be an instance");
    }
    return layoutHelper & ~LH_SLOW;
}