// Copyright 2024 solar-mist


#include <Test.h>

#include <chrono>

namespace Test
{
    struct FailedTest
    {
        std::string name;
        std::string suite;

        std::string file;
        int line;
        int col;
        
        std::string expansion;
    };

    std::vector<TestCase> tests;
    std::vector<FailedTest> failedTests;
    size_t totalAssertions;
    size_t failedAssertions;
    TestCase* currentTest;

    void TestFailed(AssertInfo info)
    {
        ++failedAssertions;

        FailedTest fail;
        fail.file = info.sourceLocation.file_name();
        fail.line = info.sourceLocation.line();
        fail.col = info.sourceLocation.column();

        fail.expansion = info.lhs;
        switch (info.op)
        {
            case AssertOperator::EQ:
                fail.expansion += " == ";
                break;
            case AssertOperator::NEQ:
                fail.expansion += " != ";
                break;
            case AssertOperator::LE:
                fail.expansion += " <= ";
                break;
            case AssertOperator::LT:
                fail.expansion += " < ";
                break;
            case AssertOperator::GE:
                fail.expansion += " >= ";
                break;
            case AssertOperator::GT:
                fail.expansion += " > ";
                break;
            default:
                break;
        }
        fail.expansion += info.rhs;

        fail.name = currentTest->name;
        fail.suite = currentTest->suite;

        failedTests.push_back(fail);
    }

    void Diagnostics(DiagnosticOutputType outputType)
    {
        if (outputType == DiagnosticOutputType::Console)
        {
            std::cout << "\n" << totalAssertions << " assertions. \x1b[;32m" << totalAssertions - failedAssertions << " passed, \x1b[;31m" << failedAssertions << " failed.\x1b[;0m\n";
            std::cout << tests.size() << " tests run. \x1b[;32m" << tests.size() - failedTests.size() << " succeeded, \x1b[;31m" << failedTests.size() << " failed.\x1b[;0m\n\n";
            for (FailedTest& test : failedTests)
            {
                std::cout << "\x1b[;31mTest " << test.suite << "::" << test.name << "(" << test.file << ":" << test.line << ":" << test.col << ") failed with expansion:\x1b[;0m\n\t" << test.expansion << "\n";
            }
            if (failedTests.empty())
            {
                std::cout << "\x1b[;32mAll tests passed.\x1b[;0m\n";
            }
            else
            {
                std::exit(1);
            }
        }
        else if (outputType == DiagnosticOutputType::Markdown)
        {
            std::cout << "# Test Results\n";
            std::cout << std::format("{} tests run. :heavy_check_mark: {} succeeded, :x: {} failed.\n", tests.size(), tests.size() - failedTests.size(), failedTests.size());
            
            for (FailedTest& test : failedTests)
            {
                std::cout << std::format("<details>\n\t<summary>:x:{}::{}</summary>\n", test.suite, test.name);
                std::cout << "| | |\n|-|-|";
                std::cout << std::format("\n| **File:**      |{}", test.file);
                std::cout << std::format("\n| **Location:**  |{}:{}", test.line, test.col);
                std::cout << std::format("\n| **Assertion:** |{}", test.expansion);
                std::cout << "</details>\n";
            }
            if (failedTests.empty())
            {
                std::cout << ":heavy_check_mark: All tests passed.\n";
            }
            else
            {
                std::cout << ":heavy_check_mark: All other tests passed.\n";
                std::exit(1);
            }
        }
    }

    void RunTests(DiagnosticOutputType outputType)
    {
        for (TestCase& test : tests)
        {
            currentTest = &test;
            const auto start = std::chrono::system_clock::now().time_since_epoch();
            test.method();
            const auto end = std::chrono::system_clock::now().time_since_epoch();

            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            if (outputType != DiagnosticOutputType::Markdown)
            {
                std::cout << test.suite << "::" << test.name << "|duration|" << duration << "\n";
            }
        }
        Diagnostics(outputType);
    }
}