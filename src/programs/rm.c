
#include "private.h"

DEFINE_MAIN_PROTO(rm, argc, argv)
{
    bool force;
    bool prompt;
    bool recursive;
    char input[32];
    if(argc == 1)
    {
        fprintf(stderr, "rm: missing operand\n");
        return 1;
    }
    force = false;
    prompt = false;
    recursive = false;
    while((argc > 1) && (argv[1][0] == '-'))
    {
        switch(argv[1][1])
        {
            case 'f':
                force = true;
                fprintf(stderr, "rm: '-f' not implemented yet\n");
                return 1;
                break;
            case 'i':
                prompt = true;
                break;
            case 'r':
                recursive = true;
                fprintf(stderr, "rm: '-r' not implemented yet\n");
                return 1;
                break;
            default:
                fprintf(stderr, "rm: invalid option -- '%c'\n", argv[1][1]);
                return 1;
                break;
        }
        ++argv;
        --argc;
    }
    if(argv[1] == NULL)
    {
        fprintf(stderr, "rm: missing operand\n");
        return 1;
    }
    if(prompt == true)
    {
        printf("rm: remove file '%s'? ", argv[1]);
        fgets(input, sizeof(input), stdin);
        if(strcmp(input, "yes"))
        {
            return 0;
        }
    }
    if(remove(argv[1]) != 0)
    {
        bail(true, "rm: cannot remove '%s'", argv[1]);
        return 1;
    }
    return 0;
}
