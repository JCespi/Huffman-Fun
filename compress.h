//Macros
#define DIE(format, ...) fprintf(stderr, format "\n", __VA_ARGS__), exit(EXIT_FAILURE)

void encode(int dump);

void decode(void);
