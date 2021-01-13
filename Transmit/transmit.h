//Author: Stanley Eisenstat, Date: (08/08/19)
//Notes:  - Header and C file made available to his last class (Fall 2020)
//        - most code is exactly what Professor Eisenstat wrote, 
//          with comments (slightly) re-worded, sporadic modification, 
//          style changes, and heavy deletion.
//        - Professor Eisenstat is the best CS professor I have ever had, and 
//          his code's integration into my program is meant as a sort of homage to
//          both his coding prowess as well as his positive influence on students.            

// Maximum allowed value of NBITS
#define NBITS_MAX (sizeof(long) * CHAR_BIT)

/* Writes to stdout the low-order and given n_bits of the given code to a bit stream. 
 * Write to stderr instead if n_bits negative, greater than allowable n_bits, or if 
 * more bits are required to represent the given code.
 * 
 * The bit stream is padded with up to 7 zero bits such that the stream is a multiple of 8, 
 * and then the stream is broken into a sequence of 8-bit/1 byte characters that are written to stdout. 
 * In the process, some low-order bits in the code are saved to a buffer for the next call 
 * to put_bits(), meaning that flush_bits() must be called at the end to output any overflowing bits.
 * @param n_bits the number of bits associated with the codeword   
 * @param code the codeword
 * */
void put_bits(int n_bits, long code);


/* Flushes out any remaining bits in the buffer with up to 7 zero bits of padding.
 * */
void flush_bits(void);



// The bit stream is assumed to be padded with up to CHAR_BIT-1 zero bits
// so that its length is a multiple of CHAR_BIT and to be broken into
// CHAR_BIT-bit characters.
//
/* Returns EOF if there are fewer than n_bits remaining or 
 * if given n_bits is negative, greater than the allowable n_bits, or the 
 * codeword returned is negative (upon which stderr is written to). 
 * Otherwise, returns an integer with high-order bits as zero and 
 * low-order bits are the next n_bits from a bit stream in stdin. 
 * The bit stream is assumed to be a multiple of 8 
 * (and as a result, padded with zero bits) and assumed to be 
 * broken into 8-bit/byte characters.
 * @param n_bits the number of bits to read from a bitstream in stdin
 * */
long get_bits(int n_bits);
