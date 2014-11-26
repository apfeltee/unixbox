
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

bool ub_opendir(struct Directory* dest, const char* path)
{
    if((dest->handle = opendir(path)) != NULL)
    {
        return true;
    }
    return false;
}

void ub_rewinddir(struct Directory* dir)
{
    rewinddir(dir->handle);
}

long ub_telldir(struct Directory* dir)
{
    return telldir(dir->handle);
}

void ub_seekdir(struct Directory* dir, long loc)
{
    seekdir(dir->handle, loc);
}

bool ub_readdir(struct Directory* dir, struct DirEntry* entry, bool skipdots)
{
    if((entry->entry = readdir(dir->handle)) != NULL)
    {
        entry->name = entry->entry->d_name;
        entry->inode = entry->entry->d_ino;
        if(skipdots)
        {
            if((strcmp(entry->name, ".") == 0) || (strcmp(entry->name, "..") == 0))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void ub_closedir(struct Directory* dir)
{
    closedir(dir->handle);
}