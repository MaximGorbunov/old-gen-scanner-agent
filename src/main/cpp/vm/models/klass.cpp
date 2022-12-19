#include "klass.h"


int Klass::getLayoutHelper() {
    return layoutHelper;
}

void Klass::printName() {
    printf("Class name: '%s' ", name->getBody().c_str());
}