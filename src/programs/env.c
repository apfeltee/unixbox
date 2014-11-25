
#include "private.h"

/*refers to envp*/
extern char **environ;

void print_env(void)
{
    char** envp;
    envp = environ;
    while(envp && *envp)
    {
        printf("%s\n", *envp);
        envp++;
    }
}

DEFINE_MAIN_PROTO(env, argc, argv)
{
    (void)argv;
    if(argc > 1)
    {
        fprintf(stderr, "not implemented yet\n");
        return 1;
    }
    print_env();
    return 0;
}
