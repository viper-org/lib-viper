// Copyright 2024 solar-mist

#ifndef VSTD_TESTS_TESTS_H
#define VSTD_TESTS_TESTS_H 1

#include "ViperAssert.h"

#include <vector>
#include <functional>
#include <iostream>
#include <source_location>

namespace Test
{
    struct TestCase
    {
        std::function<void()> method;
        std::string name;
        std::string suite;
    };

    extern std::vector<TestCase> tests;
    extern size_t totalAssertions;

    void TestFailed(AssertInfo info);
    void Diagnostics();

    void RunTests();

#define TEST(name, suite) \
    void test$##name##$##suite(); \
    struct test_##name##_##suite_registrar { \
        test_##name##_##suite_registrar() {\
            Test::tests.push_back({test$##name##$##suite, #name, #suite}); \
        } \
    }; \
    volatile test_##name##_##suite_registrar test_##name##_##suite_registrar_object; \
    void test$##name##$##suite()

#define REQUIRE(condition) \
    do { \
        ++Test::totalAssertions; \
        auto info = AssertDecomposer{AssertInfo{#condition, std::source_location::current(), AssertOperator::None, AssertResult::None}} < condition; \
        if (info.result != AssertResult::True)\
        { \
            Test::TestFailed(info); \
            return; \
        } \
    } while(0)

#define REQUIRE_FALSE(condition) \
    do { \
        ++Test::totalAssertions; \
        auto info = AssertDecomposer{AssertInfo{#condition, std::source_location::current(), AssertOperator::None, AssertResult::None}} < condition; \
        if (info.result != AssertResult::False)\
        { \
            Test::TestFailed(info); \
            return; \
        } \
    } while(0)

}

#endif // VSTD_TESTS_TESTS_H