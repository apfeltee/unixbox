
#include "private.h"

enum
{
    kChunkSize = 8192 * 8,
};

static void handle_file(const char* progname, FILE* fh, const char* filename)
{
    /* reading 8MB*8 chunks at a time make cat faster than iterating
       through each char with fgetc. it's what most cat implementations
       do, so no reason to reinvent the wheel here */
    char buffer[kChunkSize + 1];
    size_t chunksize;
    (void)filename;
    while((chunksize = fread(buffer, sizeof(char), kChunkSize, fh)) > 0)
    {
        if(fwrite(buffer, sizeof(char), chunksize, stdout) != chunksize)
        {
            bail(false, "%s: write() failed", progname);
        }
    }
}

DEFINE_MAIN_PROTO(cat, argc, argv)
{
    int i;
    const char* progname;
    FILE* handle;
    progname = argv[0];
    if(argc == 1)
    {
        handle_file(progname, stdin, "<stdin>");
        return 0;
    }
    for(i=1; i<argc; i++)
    {
        handle = fopen(argv[i], "rb");
        if(handle == NULL)
        {
            bail(false, "%s: cannot open %s", progname, argv[i]);
        }
        else
        {
            handle_file(progname, handle, argv[i]);
            fclose(handle);
        }
    }
    return 0;
}
