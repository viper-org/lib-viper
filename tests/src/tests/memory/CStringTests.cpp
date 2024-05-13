// Copyright 2024 solar-mist


#include "Test.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    void* _F3std6memcpyAbPbPi(void* p1, void* p2, int count);
}

namespace MemoryTests
{
    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

    TEST(memcpy, CStringTests)
    {
        std::unique_ptr<unsigned char[]> to   = std::make_unique<unsigned char[]>(0x1000);
        std::unique_ptr<unsigned char[]> from = std::make_unique<unsigned char[]>(0x1000);
        std::memset(to.get(), 0, 0x1000);
        
        random_bytes_engine rbe;
        std::generate(from.get(), from.get()+0x1000, std::ref(rbe));

        _F3std6memcpyAbPbPi(to.get(), from.get(), 0x1000);

        for (int i = 0; i < 0x1000; ++i)
        {
            REQUIRE(to[i] == from[i]);
        }
    }
}