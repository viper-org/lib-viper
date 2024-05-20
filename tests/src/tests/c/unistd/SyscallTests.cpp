// Copyright 2024 solar-mist

#include "../../../../include/Test.h"

#include <fcntl.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <random>

extern "C"
{
    size_t _F3std1c4readAibPl(int fd, char* buf, size_t count);
    size_t _F3std1c5writeAibPul(int fd, char* data, size_t count);
    int _F3std1c4openAbPius(const char* pathname, int flags, unsigned short mode);
    int _F3std1c5closeAi(int fd);
    int _F3std1c5lseekAiii(int fd, int offset, int whence);
}

namespace MemoryTests
{
    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;

    TEST(openReadWriteSeekAndClose , SyscallTests)
    {
        constexpr size_t bufSize = 0x1000;
        std::unique_ptr<char[]> buf = std::make_unique<char[]>(bufSize);
        
        random_bytes_engine rbe;
        std::generate(buf.get(), buf.get() + bufSize, std::ref(rbe));

        const char* tmpName = std::tmpnam(nullptr);
        int fd = _F3std1c4openAbPius(
            tmpName,
            O_RDWR | O_CREAT | O_EXCL,
            S_IRUSR | S_IWUSR);
        REQUIRE(fd != -1);
        
        size_t countWritten = _F3std1c5writeAibPul(fd, buf.get(), bufSize);
        REQUIRE(countWritten == bufSize);

        int seekedOffset = _F3std1c5lseekAiii(fd, 0, SEEK_SET);
        REQUIRE(seekedOffset == 0);

        std::unique_ptr<char[]> readBuf = std::make_unique<char[]>(bufSize);
        size_t countRead = _F3std1c4readAibPl(fd, readBuf.get(), bufSize);
        REQUIRE(countRead == bufSize);
        REQUIRE(std::memcmp(readBuf.get(), buf.get(), bufSize) == 0);

        int res = _F3std1c5closeAi(fd);
        REQUIRE(res == 0);
    }
}