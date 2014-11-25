
#include "private.h"

DEFINE_MAIN_PROTO(sleep, argc, argv)
{
    int amount;
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s [seconds]\n", argv[0]);
        return 1;
    }
    amount = atoi(argv[1]);
    sleep((size_t)amount);
    return 0;
}
