
#include "private.h"

DEFINE_MAIN_PROTO(pwd, argc, argv)
{
    if((argc > 1) && (strcmp(argv[1], "-L") == 0))
    {
        puts(getenv("PWD"));
    }
    else
    {
        puts(getcwd(NULL, 0));
    }
    return 0;
}
