#include "klass.h"


int Klass::getLayoutHelper() {
    return layoutHelper;
}

void Klass::printName() {
    printf("Class name: '%s' \n", std::string(name->getBody()));
}