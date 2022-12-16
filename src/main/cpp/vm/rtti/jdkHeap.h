#ifndef HEAP_SCANNER_JDKHEAP_H
#define HEAP_SCANNER_JDKHEAP_H
class CollectedHeap {
public:
    virtual int stub() = 0;
};

class G1CollectedHeap : public CollectedHeap {
    int stub() override = 0;
};

#endif //HEAP_SCANNER_JDKHEAP_H
