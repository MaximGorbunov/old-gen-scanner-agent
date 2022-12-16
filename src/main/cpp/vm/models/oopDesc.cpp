#include "oopDesc.h"
#include "globalDefinitions.h"
#include "alignment.h"

int OopDesc::object_size() {
    int layoutHelper = klass->getLayoutHelper();
    int size;
    if (layoutHelper > LH_NEUTRAL) {
        if ((layoutHelper & LH_SLOW) == 0) {
            size = layoutHelper;
        } else {
            size = klass->getObjectSize(addr);
        }
    } else {
        if (layoutHelper < LH_NEUTRAL) {
            int size_in_bytes;
            uint64_t lengthOffset = compressedClassPointers ? klassGapOffset : arrayTypeSize;
            int array_length = *(int *) (addr + lengthOffset);
            size_in_bytes = array_length << layout_helper_log2_element_size(layoutHelper);
            size_in_bytes += layout_helper_header_size(layoutHelper);
            size = align_up(size_in_bytes, minObjAlignmentInBytes);
        } else {
            size = klass->getObjectSize(addr);
        }
    }
    if (size == 0 || !is_aligned(size, minObjAlignmentInBytes)) {
        throw std::runtime_error("Object size is not aligned or 0: " + std::to_string(size));
    }
    return size;
}

int OopDesc::layout_helper_log2_element_size(int lh) {
    return (lh >> LH_LOG2_ELEMENT_SIZE_SHIFT) & LH_LOG2_ELEMENT_SIZE_MASK;
}

int OopDesc::layout_helper_header_size(int lh) {
    return (lh >> _lh_header_size_shift) & _lh_header_size_mask;
}

std::shared_ptr<Klass> OopDesc::getKlass() {
    return klass;
}