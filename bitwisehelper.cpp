#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bitwisehelper.hpp"
#include "stringhelper.hpp"

// reference: https://github.com/bobbyi/Fast-Bit-Counting

// The SEE implementations work in long-sized chunks
typedef const unsigned long chunk_t;
const static int chunk_size = sizeof(chunk_t);


// Iterate through the buffer one bit at a time
long count_bits_naive(const uchar *buffer, size_t bufsize)
{
    long bitcount = 0;
    for(size_t byte = 0; byte < bufsize; byte++)
        for(int bit = 0; bit < 8; bit++)
            if (buffer[byte] & (1 << bit))
                bitcount++;
    return bitcount;
}

// Count the bits using inline ASM with POPCNT for a buffer that is divisible by chunk_size
inline long count_bits_asm_chunked(const uchar *buffer, size_t bufsize)
{
    size_t iterations = bufsize / chunk_size;
    if (!iterations)
        return 0;
    // This is a dummy output variable for the bitcount
    // calculated in each iteration.
    // Which is really a temporary register that we are clobbering.
    long bitcount;
    long total;

    __asm__ (
        // do {
        "1:"
        //     bitcount = popcnt(*buffer);
        "popcnt (%[buffer]), %[bitcount];"
        //     total += bitcount;
        "add %[bitcount], %[total];"
        //     buffer += chunk_size;
        "add %[chunk_size], %[buffer];"
        // } while(--total);
        "loop 1b;"

        // Output values
        :   [total]         "=&r"       (total), 
            [bitcount]      "=&r"       (bitcount),
            // ecx and buffer are really clobbered rather than output,
            // but gcc seems to like it better if we list them here.
            [ecx]           "=&c"       (iterations), 
            [buffer]        "=&r"       (buffer)

        // Input values
        :   [chunk_size]    "i"         (chunk_size), 
                            "[buffer]"  (buffer), 
                            "[ecx]"     (iterations), 
                            "[total]"   (0)

        // Clobbered registers
        // We pretty much declared them all as outputs, so they don't
        // need to be listed again.
        :   "cc"
    );
    return total;
}

// Count the bits using inline ASM with POPCNT
long count_bits_asm(const uchar *buffer, size_t bufsize)
{
    const size_t num_chunks = bufsize / chunk_size;
    const size_t chunked_bufsize = num_chunks * chunk_size;
    const size_t leftover = bufsize - chunked_bufsize;

    long total = 0;

    const long num_bits = count_bits_asm_chunked(buffer, chunked_bufsize);
    total += num_bits;

    total += count_bits_naive(buffer + chunked_bufsize, leftover);

    return total;
}
