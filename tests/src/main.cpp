#include "Test.h"

#include <cstring>

int main(int argc, char** argv)
{
    Test::DiagnosticOutputType outputType;
    if (argc < 2)
        outputType = Test::DiagnosticOutputType::Console;
    else if (!std::strcmp(argv[1], "markdown"))
        outputType = Test::DiagnosticOutputType::Markdown;

    Test::RunTests(outputType);
}