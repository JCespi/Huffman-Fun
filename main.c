#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compress.h"

/* TO-DO
 * fix there being a little garbage that gets encoded
 *          - if you don't rewind, you can see the garbage itself
 *          - happened after making frequencies decimals and dedicating build_freq_table() to that
 * check for memory leaks by compiling on the zoo
 * add bit fields
 * add more detailed comments to header file functions. (@param, @return)
 * generalize what we can huffman encode (like being able to pass a frequency array of anything/any size)
 * add error handling to main()
 * for dumping the table:
 *      - handle multiple sources of redirection with a default
*/

int main(int argc, char **argv){
    char *path, *exectuable, *dmp_file;
    int i, dump;
    
    //get executable name
    path = argv[0];
    exectuable = &path[strlen(path) - 6];

    if (strcmp("decode", exectuable) == 0){
        if (argc > 1)
            DIE("%s", "decode: accepts no options");

        decode();
    } else {
        dump = 0;                //default dump option is false
        dmp_file = "table.txt";  //default dump file
        i = 1;

        while (i < argc){
            if (strcmp(argv[i++], "-d") == 0){
                dump = 1;

                if (i < argc)
                    dmp_file = argv[i++];
            } else 
                DIE("%s", "encode: unknown flag");
        }

        encode(dump, dmp_file);
    }

    return 0;
}
