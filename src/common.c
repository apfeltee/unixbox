
#include "private.h"

void bail(bool exit_after_print, const char* format_str, ...)
{
    int end;
    char buffptr[1024];
    va_list ap;
    va_start(ap, format_str);
    end = vsnprintf(buffptr, sizeof(buffptr), format_str, ap);
    va_end(ap);
    buffptr[end] = 0;
    perror(buffptr);
    if(exit_after_print)
    {
        exit(1);
    }
}

bool isfile(const char* path)
{
    struct stat st;
    if(stat(path, &st) == 0)
    {
        if(st.st_mode & S_IFDIR)
        {
            return true;
        }
    }
    return false;
}
