#ifndef HEAP_SCANNER_JAVALANGCLASSMETADATA_H
#define HEAP_SCANNER_JAVALANGCLASSMETADATA_H

#include <stdexcept>
#include "jvmMetadata.h"
#include "jvm.h"

class JavaLangClassMetadata : public JvmMetadata {
private:
    friend class InstanceMirrorKlass;

    std::shared_ptr<Field> klassOffset;
    std::shared_ptr<Field> arrayKlassOffset;
    std::shared_ptr<Field> oopSizeOffset;
    std::shared_ptr<Field> staticOopFieldCountOffset;
public:
    explicit JavaLangClassMetadata(const std::shared_ptr<JVM> &jvm) {
        auto typeFields = jvm->getTypeFields("java_lang_Class");
        auto _klass_offsetIterator = typeFields->find("_klass_offset");
        if (_klass_offsetIterator == typeFields->end()) {
            throw std::runtime_error("Can't find field _klass_offset in java_lang_Class");
        }
        klassOffset = _klass_offsetIterator->second;
        auto _array_klass_offsetIterator = typeFields->find("_array_klass_offset");
        if (_array_klass_offsetIterator == typeFields->end()) {
            throw std::runtime_error("Can't find field _array_klass_offset in java_lang_Class");
        }
        arrayKlassOffset = _array_klass_offsetIterator->second;
        auto _oop_size_offsetIterator = typeFields->find("_oop_size_offset");
        if (_oop_size_offsetIterator == typeFields->end()) {
            throw std::runtime_error("Can't find field _oop_size_offset in java_lang_Class");
        }
        oopSizeOffset = _oop_size_offsetIterator->second;
        auto _static_oop_field_count_offsetIterator = typeFields->find("_static_oop_field_count_offset");
        if (_static_oop_field_count_offsetIterator == typeFields->end()) {
            throw std::runtime_error("Can't find field _static_oop_field_count_offset in java_lang_Class");
        }
        staticOopFieldCountOffset = _static_oop_field_count_offsetIterator->second;
    }
};


#endif //HEAP_SCANNER_JAVALANGCLASSMETADATA_H
