
#include "private.h"

DEFINE_MAIN_PROTO(sync, argc, argv)
{
    (void)argc;
    (void)argv;
    sync();
    return 0;
}
