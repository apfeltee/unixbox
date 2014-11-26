
#include "private.h"

enum
{
    kWidth = 16,
};

static void printable(FILE* out, char* buf, unsigned int num)
{
    unsigned int i;
    for(i=0; i<num; i++)
    {
        if((buf[i] > 0x21) && (buf[i] < 0x7e))
        {
            fputc(buf[i], out);
        }
        else
        {
            fputc('.', out);
        }
    }
}

static int hexdump(FILE* out, FILE* fh, const char* filename)
{
    int i;
    int count;
    unsigned long offset;
    char buf[kWidth + 1] = {0};
    (void)filename;
    offset = 0;
    while(true)
    {
        count = fread(buf, sizeof(char), kWidth, fh);
        fprintf(out, "%07lx: ", offset);
        offset += kWidth;
        for(i=0; i<count; i++)
        {
            fprintf(out, "%02X ", (unsigned char)buf[i]);
        }
        if(count < kWidth)
        {
            for(i = 0; i < (kWidth - count); i++)
            {
                fputs("   ", out);
            }
            printable(out, buf, count);
            fputc('\n', out);
            break;
        }
        printable(out, buf, kWidth);
        fputc('\n', out);
        fflush(out);
    }
    return 0;
}

DEFINE_MAIN_PROTO(hd, argc, argv)
{
    int i;
    int rc;
    FILE* fh;
    if(argc > 1)
    {
        for(i=1; i<argc; i++)
        {
            if((!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help")))
            {
                fprintf(stderr, "usage: %s <file ...>\n", argv[0]);
                return 0;
            }
            if((fh = fopen(argv[i], "rb")) != NULL)
            {
                rc = hexdump(stdout, fh, argv[i]);
                fclose(fh);
                return rc;
            }
            else
            {
                fprintf(stderr, "failed to open '%s' for reading\n", argv[i]);
                return 1;
            }
        }
    }
    else
    {
        return hexdump(stdout, stdin, "<stdin>");
    }
    return 0;
}
