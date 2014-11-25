
#include "private.h"

DEFINE_MAIN_PROTO(tty, argc, argv)
{
    (void)argc;
    (void)argv;
    if(isatty(0) == 1)
    {
        printf("%s\n", ttyname(0));
    }
    else 
    {
        printf("not a tty");
        return 1;
    }
    return 0;
}
