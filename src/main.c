
#include "private.h"

bool get_program(struct ProgramPair* dest, const char* progname)
{
    int i;
    for(i=0; all_programs[i].name != NULL; i++)
    {
        if(strcmp(all_programs[i].name, progname) == 0)
        {
            *dest = all_programs[i];
            return true;
        }
    }
    return false;
}

int run_prog(const char* name, int argc, char** argv)
{
    struct ProgramPair prog;
    if(get_program(&prog, name))
    {
        return prog.main(argc, argv);
    }
    else
    {
        fprintf(stderr, "program '%s' is not available\n", name);
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int i;
    int linebreak;
    if(argc > 1)
    {
        return run_prog(argv[1], argc-1, argv+1);
    }
    fprintf(stderr, "usage: %s <program> [<args...>]\n", argv[0]);
    fprintf(stderr, "supported programs:\n\n  ");
    for(i=0, linebreak=0; all_programs[i].name != NULL; i++, linebreak++)
    {
        fprintf(stderr, "%s ", all_programs[i].name);
        if(linebreak == 7)
        {
            fprintf(stderr, "\n  ");
            linebreak = 0;
        }
    }
    fprintf(stderr, "\n\n");
    return 1;
}
