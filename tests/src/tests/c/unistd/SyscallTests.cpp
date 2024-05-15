// Copyright 2024 solar-mist

#include "../../../../include/Test.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    int _F3std1c5writeAibPi(int fd, char* data, int count);
}

namespace MemoryTests
{
    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

    TEST(write, SyscallTests)
    {
        std::unique_ptr<char[]> data = std::make_unique<char[]>(0x1000);
        
        random_bytes_engine rbe;
        std::generate(data.get(), data.get()+0x1000, std::ref(rbe));

        std::FILE* file = std::tmpfile();
        int fd = fileno(file);
        
        int count = _F3std1c5writeAibPi(fd, data.get(), 0x1000);
        rewind(file);

        REQUIRE(count == 0x1000);
        for (int i = 0; i < count; ++i)
        {
            REQUIRE((char)getc(file) == data[i]);
        }
    }
}