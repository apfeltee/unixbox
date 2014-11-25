
#include "private.h"

typedef enum
{
    NO_ERROR,
    ERROR_EOF,
    ERROR_MEM,
    ERROR_FILE,
} RESULT;

/*Sets the file position to just after a character 'c'*/
static RESULT skip_over_char(char c, FILE* fp)
{
    int fc;
    while(true)
    {
        fc = fgetc(fp);
        if (fc == EOF)
        {
            return ERROR_EOF;
        }
        if(fc == c)
        {
            return NO_ERROR;
        }
    }
}

static RESULT process_passwd()
{
    int tempid;
    RESULT RETURN_STATUS;
    long start;
    long end;
    size_t len;
    uid_t euid;
    char* name;
    FILE* passwd;
    euid = geteuid();
    passwd = fopen("/etc/passwd", "r");
    name = NULL;
    /*perhaps an incomplete chroot or something*/
    if(passwd == NULL)
    {
        return ERROR_FILE;
    }
    RETURN_STATUS = NO_ERROR;
    while(true)
    {
        /*record position of name*/
        start = ftell(passwd);
        /*move over ':', check for eof*/
        if(skip_over_char(':', passwd))
        {
            RETURN_STATUS = ERROR_EOF;
            break;
        }
        /*
        * record current position - one char after the ':' character
        * then move back to string, calculate length
        */
        end = ftell(passwd) - 1;
        fseek(passwd, start, SEEK_SET);
        len = ((size_t)(end - start));
        /* I image this always *should* be true */
        if(len > 0)
        {
            /*reallocate space for string, including NULL byte*/
            if(!(name = (char*)realloc(name, len+1)))
            {
                RETURN_STATUS = ERROR_MEM;
                break; 
            }
            /*read string, if it fucks up return 1 I guess*/
            if (fread(name, 1, len, passwd) != len)
            {
                RETURN_STATUS = ERROR_FILE;
                break;
            }
            name[len] = 0;
            /*move back over two colons*/
            if(skip_over_char(':', passwd) || skip_over_char(':', passwd))
            {
                RETURN_STATUS = ERROR_EOF;
                break;
            }
            /*read and check the uid*/
            tempid = -1;
            fscanf(passwd, "%d", &tempid);
            if(tempid == -1)
            {
                RETURN_STATUS = ERROR_FILE;
                break;
            }
            /*if the id's match, report the "user name" to stdout*/
            if(tempid == ((int)euid))
            {
                printf("%s\n", name);
                RETURN_STATUS = NO_ERROR;
                break;
            }
        } 
        /*
        * if there is no name before uid (len == 0) then
        * there's a possible formatting error in the passwd file
        */
        else
        {
            RETURN_STATUS = ERROR_FILE;
            break;
        }
        /*skip to next line, check for EOF*/
        if(skip_over_char('\n', passwd))
        {
            RETURN_STATUS = ERROR_EOF;
            break;
        }
    }
    if(name)
    {
        free(name);
    }
    return RETURN_STATUS;
}

static void print_usage(const char* progname)
{
    printf("Usage %s [OPTION]...\n", progname); 
    printf("Print the user name associated with the current effective user ID.\n");
    printf("Same as id -un.\n");
    printf("\n");
    printf("\t--help     display this help and exit\n");
    printf("\t--version  output version information and exit\n");
}

static void print_version()
{
    printf("whoami: part of unixbox\n");
}

static bool find_argument(const char* arg, const char* pattern)
{
    if(strlen(arg) == (strlen(pattern) + 2))
    {
        if((arg[0] == '-') && (arg[1] == '-'))
        {
            if (!strcmp(pattern, &arg[2]))
            {
                return true;
            }
        }
    }
    return false;
}

DEFINE_MAIN_PROTO(whoami, argc, argv)
{
    char** arg;
    if(argc > 1)
    {
        arg = argv;
        while(*(++arg))
        {
            if(find_argument(*arg, "help"))
            {
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            }
            else if (find_argument(*arg, "version"))
            {
                print_version();
                return EXIT_SUCCESS;
            }
            else
            {
                printf("%s: extra operand `%s’\n", argv[0], *arg);
            }
        }
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    switch(process_passwd())
    {
        case NO_ERROR:
            return EXIT_SUCCESS;
        case ERROR_EOF:
            fprintf(stderr, "Premature EOF, check your /etc/passwd!\n");
            return EXIT_FAILURE;
        case ERROR_FILE:
            fprintf(stderr, "Error occured when reading /etc/passwd!\n");
            return EXIT_FAILURE;
        case ERROR_MEM:
            fprintf(stderr, "Encountered a memory error!\n");
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
