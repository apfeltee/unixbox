
#include "private.h"

DEFINE_MAIN_PROTO(arch, argc, argv)
{
    struct utsname name;
    (void)argc;
    (void)argv;
    if(uname(&name) == 0)
    {
        puts(name.machine);
        return 0;
    }
    return -1;
}
