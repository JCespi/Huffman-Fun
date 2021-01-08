#include <stdlib.h>
#include <string.h>
#include "transmit.h"

// Information shared by putBits() and flushBits()
static int n_extra_bits = 0;            // num of bits from previous byte(s)
static unsigned long extra_bits = 0;     // Extra bits from previous byte(s)

// Write parameter code of size n_bits to standard output
void put_bits (int n_bits, long code){
	unsigned long max_code, c;

    if (n_bits <= 0 || NBITS_MAX < n_bits) {              // NBITS valid?
		  fprintf(stderr, "invalid n_bits = %d\n", n_bits);
		  return;
    }

    max_code = 1UL << (n_bits-1);           			// Compute maximum code
    max_code = max_code + (max_code-1);

    if (code < 0 || max_code < code) {                   // CODE representable?
		  fprintf(stderr, "nBits = %d, maxCode = %lu, code = %ld\n", n_bits, max_code, code);
		  return;
    }

    // If EXTRABITS << NBITS could overflow, output high-order CHAR_BIT bits
    if (n_bits > NBITS_MAX - CHAR_BIT) {
		  put_bits(CHAR_BIT, code >> (n_bits-CHAR_BIT));
		  n_bits -= CHAR_BIT;
    }

    n_extra_bits += n_bits;                                    // Add bits to EXTRABITS
    extra_bits = (extra_bits << n_bits) | code;

    while (n_extra_bits >= CHAR_BIT) {                        // Output whole chars &
      n_extra_bits -= CHAR_BIT;                             //  save remaining bits
      c = extra_bits >> n_extra_bits;
      putchar (c);
      extra_bits ^= c << n_extra_bits;
    }
}

// Flush remaining bits to standard output
unsigned flush_bits (void){
	if (n_extra_bits != 0)
	    putchar(extra_bits << (CHAR_BIT - n_extra_bits));
    return n_extra_bits;
}

// Return next code (#bits = NBITS) from input stream or EOF on end-of-file
long get_bits (int n_bits){
	unsigned long max_code;
	long result;

    if (n_bits <= 0 || NBITS_MAX < n_bits) {              // NBITS valid?
		  fprintf (stderr, "invalid n_bits = %d\n", n_bits);
		  return EOF;
    }

    max_code = 1 << (n_bits-1);             // Maximum possible code
    max_code = max_code + (max_code-1);

    static int n_extra_bits = 0;         // #bits from previous byte(s)
    static unsigned long extra_bits = 0; // Extra bits from previous byte(s)
									      
    result = 0;                    // Value to return

    // If EXTRABITS << NBITS could overflow, read high-order CHAR_BIT bits
    if (n_bits > NBITS_MAX - CHAR_BIT) {
      result = get_bits (CHAR_BIT);
      if (result < 0)
        return EOF;
      n_bits -= CHAR_BIT;
    }

    // Read enough new bytes to have at least NBITS bits to extract code
    long c;
    while (n_extra_bits < n_bits) {
      if ((c = getchar()) == EOF)                     // Too few bits?
        return EOF;
      n_extra_bits += CHAR_BIT;
      extra_bits = (extra_bits << CHAR_BIT) | c;
    }

    // Extract remaining bits and save remainder
    n_extra_bits -= n_bits;
    c = extra_bits >> n_extra_bits;
    extra_bits ^= c << n_extra_bits;
    return (result << n_bits) | c;
}
