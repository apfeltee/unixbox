
#include "private.h"

DEFINE_MAIN_PROTO(mv, argc, argv) 
{
    int status;
    char *input;
    char *output;
    if(argc < 2) 
    {
        fprintf(stderr, "mv: missing file operand\n");
        return(1);
    }
    if(argc < 3)
    {
        fprintf(stderr, "mv: missing file operand after '%s'\n", argv[1]);
        return(1);
    }
    input = argv[1];
    output = argv[2];
    if(isfile(input) && isfile(output))
    {
        status = rename(input, output);
        if(status == -1)
        {
            bail(true, "mv: rename() from '%s' to '%s' failed", input, output);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "mv: not implemented yet\n");
    }
    return(0);
}
