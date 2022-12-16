#ifndef HEAP_SCANNER_GLOBALDEFINITIONS_H
#define HEAP_SCANNER_GLOBALDEFINITIONS_H

#define LogBytesPerShort   1
#define LogBytesPerInt     2
#ifdef _LP64
#define LogBytesPerWord    3
#else
#define LogBytesPerWord    2
#endif
#define LogBytesPerLong    3
#define LogBitsPerByte 3


const int BytesPerShort = (1 << LogBytesPerShort);
const int BytesPerInt = (1 << LogBytesPerInt);
const int BytesPerWord = (1 << LogBytesPerWord);
const int BytesPerLong = (1 << LogBytesPerLong);

const int LogBitsPerShort = (LogBitsPerByte + LogBytesPerShort);
const int LogBitsPerInt = (LogBitsPerByte + LogBytesPerInt);
const int LogBitsPerWord = (LogBitsPerByte + LogBytesPerWord);
const int LogBitsPerLong = (LogBitsPerByte + LogBytesPerLong);

const int BitsPerByte = (1 << LogBitsPerByte);
const int BitsPerShort = (1 << LogBitsPerShort);
const int BitsPerInt = (1 << LogBitsPerInt);
const int BitsPerWord = (1 << LogBitsPerWord);
const int BitsPerLong = (1 << LogBitsPerLong);

#ifdef _LP64
#define logHeapWordSize 3
#else
#define logHeapWordSize 2
#endif

#define LH_NEUTRAL 0
#define LH_SLOW 0x01
#define  OneBit 1
const int LH_LOG2_ELEMENT_SIZE_SHIFT = (BitsPerByte * 0);
const int LH_LOG2_ELEMENT_SIZE_MASK = (BitsPerLong - 1);

#define nth_bit(n)        (((n) >= BitsPerWord) ? 0 : (OneBit << (n)))
#define right_n_bits(n)   (nth_bit(n) - 1)

static const int _lh_header_size_shift = BitsPerByte * 2;
static const int _lh_header_size_mask = right_n_bits(BitsPerByte);

#endif //HEAP_SCANNER_GLOBALDEFINITIONS_H
