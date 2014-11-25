
#include "private.h"

#define BUFSIZE 8192

const char* program;

static int do_copy(const char* from, const char* to, struct stat source_st)
{
    int mode;
    int fold;
    int fnew;
    int n;
    char* p_from;
    char* p_to;
    char* p_buf;
    char iobuf[BUFSIZE+1];
    struct stat target_st;
    if((fold = open(from, O_RDONLY)) < 0)
    {
        bail(true, "%s: open(%s) failed", program, from);
        return 1;
    }
    fstat(fold, &target_st);
    mode = (int)(target_st.st_mode);
    if((stat(to, &source_st) >= 0) && ((source_st.st_mode & S_IFMT) == S_IFDIR))
    {
        p_from = (char*)from;
        p_to = (char*)to;
        p_buf = iobuf;
        while((*p_buf++ = *p_to++) != 0)
        {
            /* nop */;
        }
        p_buf[-1] = '/';
        p_to = p_buf;
        while((*p_buf = *p_from++) != 0)
        {
            if(*p_buf++ == '/')
            {
                p_buf = p_to;
            }
        }
        to = iobuf;
    }
    if(stat(to, &source_st) >= 0)
    {
        if((target_st.st_dev == source_st.st_dev) && (target_st.st_ino == source_st.st_ino))
        {
            bail(true, "%s: cannot copy file to itself", program);
            return 1;
        }
    }
    if((fnew = creat(to, (mode_t)mode)) < 0)
    {
        bail(true, "%s: cannot create %s", program, to);
        close(fold);
        return 1;
    }
    while((n = read(fold,  iobuf,  BUFSIZE)))
    {
        if(n < 0)
        {
            bail(true, "%s: read() failed", program);
            close(fold);
            close(fnew);
            return 1;
        }
        else
        {
            if(write(fnew, iobuf, (size_t)n) != n)
            {
                bail(true, "%s: write() failed", program);
                close(fold);
                close(fnew);
                return 1;
            }
        }
    }
    close(fold);
    close(fnew);
    return 0;
}

DEFINE_MAIN_PROTO(cp, argc, argv)
{
    int i;
    int copy_count;
    const char* from;
    const char* to;
    const char* filename;
    struct stat source_st;
    program = argv[0];
    if (argc < 3)
    {
        fprintf(stderr,
            "Usage: %s f1 f2\n"
            "       %s f1 ... fn d2\n", program, program);
        return 1;
    }
    if (argc > 3)
    {
        filename = argv[argc - 1];
        if (stat(filename, &source_st) < 0)
        {
            bail(true, "stat(%s) failed", filename);
        }
        if ((source_st.st_mode & S_IFMT) != S_IFDIR)
        {
            bail(true, "file '%s' is a directory", filename);
        }
    }
    copy_count = 0;
    for(i=1; i<argc-1; i++)
    {
        from = argv[i];
        to = argv[argc-1];
        copy_count |= do_copy(from, to, source_st);
    }
    return copy_count;
}

