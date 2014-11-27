
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cctype>
#include <math.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(__unix__) || defined(__linux__) || defined(__CYGWIN__) || defined(__TINYC__)
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/utsname.h>
    #include <dirent.h>
    #define UB_ISUNIX          1
    #define G_O_BINARY         0
    #define g_open(P, F, M)    open(P, F, M)
    #define g_read(F, B, N)    read(F, B, N)
    #define g_write(F, B, N)   write(F, B, N)
    #define g_lseek(F, O, W)   lseek(F, O, W)
    #define g_close(F)         close(F)
    #define g_mkdir(P, M)      mkdir(P, M)
    #define g_rmdir(P)         rmdir(P)
    #define g_unlink(P)        unlink(P)
#elif defined(_WIN32)
    #include <windows.h>
    #include <io.h>
    #include <features.h>
    #include <fcntl.h>
    #define G_O_BINARY         O_BINARY
    #define g_open(P, F, M)    _open(P, F, M)
    #define g_read(F, B, N)    _read(F, B, N)
    #define g_write(F, B, N)   _write(F, B, N)
    #define g_lseek(F, O, W)   _lseek(F, O, W)
    #define g_close(F)         _close(F)
    #define g_mkdir(P, M)      _mkdir(P)
    #define g_rmdir(P)         _rmdir(P)
    #define g_unlink(P)        _unlink(P)
#else
    #error "your platform isn't supported yet!"
    #include </platform_not_supported/>
#endif

#define DEFINE_MAIN_PROTO(name, argcname, argvname) \
    extern "C" int impl_##name##_main(int argcname, char** argvname)

typedef int(*ProgramEntry)(int, char**);

struct Directory
{
    DIR* handle;
    const char* dirname;
};

struct DirEntry
{
    struct dirent* entry;
    const char* name;
    ino_t inode;
    unsigned char type;
};


struct ProgramPair
{
    const char* name;
    const char* description;
    ProgramEntry main;
};

/**
* ----------------------
* -- helper functions --
* ----------------------
*/

/* uses perror() with vsnprintf'd format string. Internal buffer is hardcoded to 1024. */
void bail(bool exit_after_print, const char* format_str, ...);

/* uses stat to determine whether $path is a file. */
bool isfile(const char* path);

/* uses stat to determine whether $path is a directory. */
bool isdirectory(const char* path);

bool parse_flag(const char* str, const char* name, const char* dest);

/*
the following functions wrap dirent.h (in case some OSs implement some funky things ...)

DT_REG A     regular file.
DT_DIR A     directory.
DT_FIFO A    named pipe, or FIFO. See FIFO Special Files.
DT_SOCK A    local-domain socket.
DT_CHR A     character device.
DT_BLK A     block device.
DT_LNK A     symbolic link.
DT_UNKNOWN   The type is unknown. Only some filesystems have full support
             to return the type of the file, others might always return this value.
*/
bool ub_opendir(struct Directory* dest, const char* path);
void ub_rewinddir(struct Directory* dir);
long ub_telldir(struct Directory* dir);
void ub_seekdir(struct Directory* dir, long loc);
bool ub_readdir(struct Directory* dir, struct DirEntry* entry, bool skipdots);
void ub_closedir(struct Directory* dir);
