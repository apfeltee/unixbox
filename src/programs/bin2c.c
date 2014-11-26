
#include "private.h"

#define BIN2C_PREFIX "bin2c_"

static void processhandle(const char* fieldname, FILE* inhandle, FILE* out)
{
    int current;
    int size = 0;
    fprintf(out, "static const unsigned char %s%s_data[] =\n{\n", BIN2C_PREFIX, fieldname);
    while(1)
    {
        fprintf(out, "    ");
        while((current = fgetc(inhandle)) != EOF)
        {
            size++;
            fprintf(out, "0x%.4x,", current);
            if((size % 10) == 0)
            {
                break;
            }
        }
        if(current == EOF)
        {
            break;
        }
        else
        {
            fprintf(out, "\n");
        }
    }
    fprintf(out, "\n};\n");
    fprintf(out, "static const unsigned int %s%s_size = %d;\n\n", BIN2C_PREFIX, fieldname, size);
}

static FILE* open_or_error(const char* path, const char* mode, const char* act)
{
    FILE* ret;
    ret = fopen(path, mode);
    if(ret == NULL)
    {
        fprintf(stderr, "Could not open '%s' for %s: %s\n", path, act, strerror(errno));
        return NULL;
    }
    return ret;
}

DEFINE_MAIN_PROTO(bin2c, argc, argv)
{
    const char* fieldname;
    const char* filename;
    const char* outfile;
    FILE* handle;
    FILE* outhandle;
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <fieldname> <filename> [<outputfile>]\n", argv[0]);
        return 1;
    }
    else
    {
        fieldname = argv[1];
        filename = argv[2];
        if(argc > 3)
        {
            outfile = argv[3];
            if((outhandle = open_or_error(outfile, "w+", "writing")) == NULL)
            {
                return 1;
            }
        }
        else
        {
            outhandle = stdout;
        }
        if((handle = open_or_error(filename, "rb", "reading")) == NULL)
        {
            return 1;
        }
        processhandle(fieldname, handle, outhandle);
        fflush(outhandle);
        fclose(handle);
        fclose(outhandle);
    }
    return 0;
}



