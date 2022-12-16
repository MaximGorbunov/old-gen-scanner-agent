#ifndef HEAP_SCANNER_ALIGNMENT_H
#define HEAP_SCANNER_ALIGNMENT_H

#include <cstdio>

static inline int alignment_mask(int alignment) {
    return alignment - 1;
}

constexpr int align_down(int size, int alignment) {
    // Convert mask to T before logical_not.  Otherwise, if alignment is unsigned
    // and smaller than T, the result of the logical_not will be zero-extended
    // by integral promotion, and upper bits of size will be discarded.
    int result = size & ~int(alignment_mask(alignment));
    return result;
}


static inline int align_up(int size, int alignment) {
    int adjusted = size + alignment_mask(alignment);
    return align_down(adjusted, alignment);
}

bool is_aligned(int size, int alignment) {
    return (size & alignment_mask(alignment)) == 0;
}


#endif //HEAP_SCANNER_ALIGNMENT_H
