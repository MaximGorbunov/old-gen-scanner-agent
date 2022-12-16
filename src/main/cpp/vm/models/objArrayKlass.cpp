#include "objArrayKlass.h"

int ObjArrayKlass::getObjectSize(char *addr) {
    uint32_t array_length = *(uint32_t *) (addr + lengthOffset);
}