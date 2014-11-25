
#include "private.h"

struct arg_list
{
    int show_hidden;
    char** dir_list;
};

static int no_dot(const struct dirent* ent)
{
    if(ent->d_name[0] != '.')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int one(const struct dirent* ent)
{
    (void)ent;
    return 1;
}

static void itr_ent(char* dir_name, struct arg_list args)
{
    int i;
    int d;
    int n;
    char* dir;
    struct dirent **eps;
    // Iterate over each directory in arg_list
    i = 0;
    while(true)
    {
        dir = args.dir_list[i];
        if(dir == NULL)
        {
            if(i == 0)
            {
                dir = "./";
            }
            else
            {
                break;
            }
        }
        if(args.show_hidden == 1)
        {
            d  = scandir(dir, &eps, one, alphasort);
        }
        else
        {
            d = scandir(dir, &eps, no_dot, alphasort);
        }
        if(d >= 0)
        {
            for(n = 0; n < d; n++)
            {
                printf("%s  ", eps[n]->d_name);
            }
            printf("\n");
        }
        else
        {
            printf("ls: cannot access %s: %s\n", dir_name, strerror(errno));
        }
        i++;
    }
}

DEFINE_MAIN_PROTO(ls, argc, argv)
{
    int i;
    int j;
    size_t n;
    size_t slen;
    char** dl;
    struct arg_list args = {0, NULL};
    //Points to the locations of directories in argv
    dl = (char**)malloc((size_t)(argc) * sizeof(char**));
    j = 0;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            slen = strlen(argv[i]);
            //Skip "-" and move to flag content
            for(n = 1; n < slen; n++)
            {
                switch(argv[i][n])
                {
                    // -a: Show .*
                    case 'a':
                        args.show_hidden = 1;
                }
            } 
        }
        else
        {
            dl[j] = argv[i];
            j++;
        }
    }
    dl[j] = NULL;
    args.dir_list = dl;
    itr_ent(argv[1], args);
    return 0;
}

