#ifndef BITWISEHELPER_H
#define BITWISEHELPER_H

#include <cstdlib>

typedef unsigned char uchar;

long count_bits_naive(const uchar *buffer, size_t bufsize);

inline long count_bits_asm_chunked(const uchar *buffer, size_t bufsize);

long count_bits_asm(const uchar *buffer, size_t bufsize);

#endif
