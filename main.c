#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compress.h"

//Macros
#define DIE(format, ...) fprintf(stderr, format "\n", __VA_ARGS__), exit(EXIT_FAILURE)

/* TO-DO
 * add bit fields
 * add more detailed comments to header file functions. (@param, @return)
 * add error handling to main()
 * add bit error checks somehow
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
