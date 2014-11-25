
#include "private.h"

static void help(const char* p)
{
    printf(
        "Destroys a file.\n"
        "Usage: %s FILE\n",
        p
    );
}

DEFINE_MAIN_PROTO(unlink, argc, argv)
{
    if (argc == 1)
    {
        help(argv[0]);
        return 0;
    }
    if (g_unlink(argv[1]))
    {
        bail(true, "unlink: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}
