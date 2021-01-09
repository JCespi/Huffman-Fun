//Jeacy Espinoza (01/09/2021)

/* Reads from stdin to form a frequency table, which is used to generate a huffman code and 
 * a huffman tree. The huffman tree is put into a bistream on stdout, followed by a sequence 
 * of codewords generated by revisiting stdin.
 * @param dump      flag to determine whether to dump a table to dmp_file
 * @param dmp_file  the file to dump to in the case dump is set to high
 * */
void encode(int dump, char *dmp_file);

/* Reads from stdin, expecting a huffman tree, and builds the huffman tree. Proceeds 
 * to read a bistream representing a sequence of codewords, using the tree to decode 
 * the codewords, and outputting the character they represent to stdout. 
 * */
void decode(void);
