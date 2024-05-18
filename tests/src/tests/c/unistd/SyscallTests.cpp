// Copyright 2024 solar-mist

#include "../../../../include/Test.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    int _F3std1c5writeAlbPl(int fd, char* data, int count);
}

namespace MemoryTests
{
    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

    TEST(write, SyscallTests)
    {
        constexpr std::size_t size = 0x1000;
        std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
        
        random_bytes_engine rbe;
        std::generate(data.get(), data.get() + size, std::ref(rbe));

        std::FILE* file = std::tmpfile();
        int fd = fileno(file);
        
        int count = _F3std1c5writeAlbPl(fd, data.get(), size);
        rewind(file);

        REQUIRE(count == size);
        for (int i = 0; i < count; ++i)
        {
            REQUIRE((char)getc(file) == data[i]);
        }
    }
}