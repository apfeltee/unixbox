
#include "private.h"

#ifndef VERSION
    #define VERSION      "unknown"
#endif

#ifndef COMPILE_DATE
    #define COMPILE_DATE "unknown"
#endif

#define LICENSE      "MIT"


/* 
 * The following struct is responsible
 * for handling command line arguments
 * in a simple way (linear search)
 */

typedef struct
{
    char const *name;
    void (*function)(char** str, int argn);
} ArgList;

FILE **g_files    = NULL;
char **g_fileName = NULL;
int    g_numFiles = 0;

const char g_writeOverwrite[] = "w";
const char g_writeAppend[]    = "a";
char const * g_writeMode = g_writeOverwrite;

/*helper functions*/

static int openfiles()
{
    int i;
    /* go through all the filenames in "g_fileName" and create FILE
     * structures for writing to*/
    /* first allocate that shit */
    g_files = (FILE**)malloc(sizeof(FILE**) * g_numFiles);
    /* now open shit up */
    for (i = 0; i < g_numFiles; ++i)
    {
        /* check for stdout first */
        if (!strcmp(g_fileName[i], "-"))
        {
            g_files[i] = stdout;
        }
        /* it's just a plain old file */
        else
        {
            g_files[i] = fopen(g_fileName[i], g_writeMode);
            if (!g_files[i])
            {
                fprintf(stderr, "failed to open %s\n", g_fileName[i]);
                return 1;
            }
        }
    }
    return 0;
}

static int addfile(const char* str)
{
    int len;
    int index;
    ++g_numFiles;
    g_fileName =  (char**)realloc(g_fileName, g_numFiles * sizeof(char**));
    index = g_numFiles-1;
    len   = strlen(str);
    /* allocate string and copy over "str" (NULL byte included */
    g_fileName[index] = (char*)malloc(len+1);
    memcpy(g_fileName[index], str, len+1);
    return 0;
}

static void closefiles()
{
    int i;
    for (i = 0; i < g_numFiles; ++i)
    {
        if (g_files[i] != stdout)
        {
            fclose(g_files[i]);
        }
        if (g_fileName[i])
        {
            free(g_fileName[i]);
        }
    }
    free(g_files);
    free(g_fileName);
}

/*param handlers*/

/*ingore interrupt signals*/
static void set_ignore(char** str, int argn)
{
    (void)str;
    (void)argn;
    /*todo lel*/   
}

static void set_append(char** str, int argn)
{
    (void)str;
    (void)argn;
    g_writeMode = g_writeAppend;
}

static void add_file(char** str, int argn)
{
    addfile(str[argn]);
}

static void print_usage(char** str, int argn)
{
    (void)argn;
    printf(
        "Usage: %s [OPTION]... [FILE]...\n"
        "Copy standard input to each FILE, and also to standard output.\n"
        "  -a, --apend               append to the given FILEs, do not overwrite\n"
        "  -i, --ignore-interrupts   ignore unterrupt signals\n"
        "      --help     display this help and exit\n"
        "      --version  output version information and exit\n\n"
        ,str[0]
    );
    exit(0);
}

static void print_version(char** str, int argn)
{
    (void)str;
    (void)argn;
    printf("tee - part of unixbox\n");
    exit(0);
}


static const ArgList ArgInfo[] =
{
    {"--version", &print_version},
    {"--help",    &print_usage},
    {"-a",        &set_append},
    {"--append",  &set_append},
    {"-i",        &set_ignore},
    {"--ignore",  &set_ignore},
    {"",          &add_file},
    {NULL,        0}
};

static void handle_arguments(char** argv)
{
    int i;
    int argn;
    char** arg;
    const char* name;
    arg = argv;
    argn = 0;
    while (*(++arg))
    {
        ++argn;
        /* check for parameters */
        for (i = 0; ArgInfo[i].function; ++i)
        {
            name = ArgInfo[i].name;
            if (!strncmp(*arg, name, strlen(name)))
            {
                /* call the function with all agruments
                 * and integer denoting the current argument*/
                ArgInfo[i].function(argv, argn);
                break;
            }
        }

    }
}

DEFINE_MAIN_PROTO(tee, argc, argv)
{
    int i;
    char buf[512];
    if (argc > 1)
    {
        handle_arguments(argv);
    }
    /*enable stdout by default*/
    addfile("-");
    openfiles();
    memset(buf, 0, 512);
    while (!feof(stdin))
    {
        fgets(buf, 512, stdin);
        for (i = 0; i < g_numFiles; ++i)
        {
            fprintf(g_files[i], "%s", buf);
        }
    }
    closefiles();
    return 0;
}
