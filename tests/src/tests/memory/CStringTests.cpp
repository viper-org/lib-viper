// Copyright 2024 solar-mist

#include "Test.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    void* _F3std1c6memchrAbPii(const void* str, int c, int count);
    int _F3std1c6memcmpAbPbPi(const void* str1, const void* str2, int count);
    void* _F3std1c6memcpyAbPbPi(void* dest, void* src, int count);
    void* _F3std1c6memsetAbPii(void* ptr, int value, int count);
    int _F3std1c6strcmpAbPbP(const char* str1, const char* str2);
}

namespace MemoryTests
{
    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

    TEST(memchr, CStringTests)
    {
        int c = 'h';
        const char* str = "solar mist hello world";
        void* chr = _F3std1c6memchrAbPii(str, c, 22);

        REQUIRE(chr == str + 11);

        str = "viper is the best programming language ong";
        chr = _F3std1c6memchrAbPii(str, c, 43);

        REQUIRE(chr == nullptr);
    }

    TEST(memcmp, CStringTests)
    {
        const char* s1 = "solar";
        const char* s2 = "solar";
        int cmp = _F3std1c6memcmpAbPbPi(s1, s2, 5);
        REQUIRE(cmp == 0);

        s1 = "solar\0\0\0\0\0";
        s2 = "solar mist";
        cmp = _F3std1c6memcmpAbPbPi(s1, s2, 10);
        REQUIRE(cmp < 0);

        s1 = "solar mist";
        s2 = "solar\0\0\0\0\0";
        cmp = _F3std1c6memcmpAbPbPi(s1, s2, 10);
        REQUIRE(cmp > 0);

        s1 = "shello";
        s2 = "solar\0";
        cmp = _F3std1c6memcmpAbPbPi(s1, s2, 6);
        REQUIRE(cmp < 0);

        s1 = "solar";
        s2 = "solar";
        cmp = _F3std1c6memcmpAbPbPi(s1, s2, 2);
        REQUIRE(cmp == 0);

        std::unique_ptr<unsigned char[]> str1 = std::make_unique<unsigned char[]>(0x1000);
        std::unique_ptr<unsigned char[]> str2 = std::make_unique<unsigned char[]>(0x1000);
        std::memset(str1.get(), 69, 0x1000);
        std::memset(str2.get(), 69, 0x1000);

        str2.get()[0xFFF] = 42;

        cmp = _F3std1c6memcmpAbPbPi(str1.get(), str2.get(), 0x1000);
        REQUIRE(cmp > 0);
    }

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
        const char* str1 = "solar";
        const char* str2 = "solar";
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