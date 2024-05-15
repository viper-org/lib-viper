// Copyright 2024 solar-mist

#include "Test.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    void* _F3std1c6memcpyAbPbPi(void* dest, void* src, int count);
    void* _F3std1c6memsetAbPii(void* ptr, int value, int count);
    int _F3std1c6strcmpAbPbP(char* str1, char* str2);
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

        _F3std1c6memcpyAbPbPi(to.get(), from.get(), 0x1000);

        for (int i = 0; i < 0x1000; ++i)
        {
            REQUIRE(to[i] == from[i]);
        }
    }

    TEST(memset, CStringTests)
    {
        std::unique_ptr<unsigned char[]> ptr = std::make_unique<unsigned char[]>(0x1000);

        std::memset(ptr.get(), 0, 0x1000);

        _F3std1c6memsetAbPii(ptr.get(), 0x69, 0x1000);

        for (int i = 0; i < 0x1000; ++i)
        {
            REQUIRE(ptr[i] == 0x69);
        }
    }

    TEST(strcmp, CStringTests)
    {
        char* str1 = "solar";
        char* str2 = "solar";
        int cmp = _F3std1c6strcmpAbPbP(str1, str2);
        REQUIRE(cmp == 0);

        str1 = "solar";
        str2 = "solar mist";
        cmp = _F3std1c6strcmpAbPbP(str1, str2);
        REQUIRE(cmp < 0);

        str1 = "solar mist";
        str2 = "solar";
        cmp = _F3std1c6strcmpAbPbP(str1, str2);
        REQUIRE(cmp > 0);

        str1 = "hello";
        str2 = "solar";
        cmp = _F3std1c6strcmpAbPbP(str1, str2);
        REQUIRE(cmp < 0);

        str1 = "";
        str2 = "";
        cmp = _F3std1c6strcmpAbPbP(str1, str2);
        REQUIRE(cmp == 0);
    }
}