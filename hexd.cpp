
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

enum
{
    kLineSize = 16,
};


struct Hexdump
{
    int ch;
    size_t count = 0;
    std::istream& stream;

    Hexdump(std::istream& strm): stream(strm)
    {
        dump();
    }

    void dump()
    {
        for(; (ch = stream.get()) != EOF; count++)
        {
            std::cout << char(ch);
        }
        std::cout << std::flush;
    }
};

int main(int argc, char* argv[])
{
    int i;
    if(argc > 1)
    {
        for(i=1; i<argc; i++)
        {
            std::fstream strm(argv[i], std::ios::in | std::ios::binary);
            if(strm.good())
            {
                Hexdump{strm};
            }
            else
            {
                std::cerr << "failed to open '" << argv[i] << "' for reading\n";
                return 1;
            }
        }
    }
    else
    {
        Hexdump{std::cin};
    }
    return 0;
}
