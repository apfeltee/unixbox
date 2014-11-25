
#include "private.h"

#define ADD_ESCAPE(findme, addme) \
    case findme: \
        putchar(addme); \
        break;

static void handle_backslashes(const char* instr, size_t len)
{
    int c;
    (void)len;
    while ((c = *instr++))
    {
        if (c == '\\' && *instr)
        {
            switch (c = *instr++)
            {
                /*
                *   TODO: handle \xHH and \nnn
                */
                ADD_ESCAPE('t', '\t')
                ADD_ESCAPE('a', '\a')
                ADD_ESCAPE('b', '\b')
                ADD_ESCAPE('n', '\n')
                ADD_ESCAPE('r', '\r')
                ADD_ESCAPE('v', '\v')
                ADD_ESCAPE('e', 033)
                ADD_ESCAPE('0', '\0')
                ADD_ESCAPE('\\', '\\')
                ADD_ESCAPE('"', '\"')
                ADD_ESCAPE('\'', '\'')
            }
        }
        else if(c)
        {
            putchar(c);
        }
    }
}

DEFINE_MAIN_PROTO(echo, argc, argv)
{
    int i;
    bool omit_nl;
    bool backslashes;
    size_t arglen;
    const char* argstr;
    backslashes = false;
    omit_nl = false;
    while ((argc > 1) && (argv[1][0] == '-'))
    {
        switch (argv[1][1])
        {
            case 'e':
                backslashes = true;
                break;
            case 'n':
                omit_nl = true;
                break;
            default:
                break;
        }
        ++argv;
        --argc;
    }
    for(i=1; i<argc; i++)
    {
        argstr = argv[i];
        arglen = strlen(argstr);
        if(backslashes)
        {
            handle_backslashes(argstr, arglen);
        }
        else
        {
            printf("%s", argstr);
        }
        if((i+1) < argc)
        {
            putchar(' ');
        }
    }
    if(!omit_nl)
    {
        printf("\n");
    }
    return 0;
}

