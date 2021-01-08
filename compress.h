//Macros
#define DIE(format, ...) fprintf(stderr, format "\n", __VA_ARGS__), exit(EXIT_FAILURE)

//read file to form frequency table, generate huffman code and tree, send tree and codewords
void encode(int dump);

//read the huffman tree, read codewords and output corresponding letters
void decode(void);
