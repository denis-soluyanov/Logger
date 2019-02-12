#include "FileLogger.h"
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace dlog
{
    int FileLogger::write(const void * buf, size_t len) noexcept
    {
        if (fd_ < 0) return -1;
#ifdef _WIN32
        return ::_write(fd_, buf, static_cast<unsigned int>(len));
#else
        return ::write(fd_, buf, len);
#endif
    }

    off_t FileLogger::seek(off_t offset, int origin) noexcept
    {
        if (fd_ < 0) return -1;
#ifdef _WIN32
        return ::_lseek(fd_, offset, origin);
#else
        return ::lseek(fd_, offset, origin);
#endif
    }

    FileLogger::FileLogger() : fd_(-1) 
    {
    }

    bool FileLogger::openFile(const std::string& filename) noexcept
    {
        LockGuard guard(mtx_);
        if (fd_ > 0) closeFile();
#ifdef _WIN32
        ::_sopen_s(&fd_, filename.c_str(), _O_CREAT | _O_WRONLY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
#else
        fd_ = ::open(filename.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
        return (seek(0L, SEEK_END) != -1) ? true : false;
    }

    void FileLogger::closeFile() noexcept
    {
        LockGuard guard(mtx_);
        if (fd_ > 0) {
#ifdef _WIN32
            ::_close(fd_);
#else
            ::close(fd_);
#endif
            fd_ = -1;
        }
    }

    int FileLogger::writeInFile(const std::string& record) noexcept
    {
        LockGuard guard(mtx_);
        return write(record.c_str(), record.size());
    }

    off_t FileLogger::getFileSize() noexcept
    {
        LockGuard guard(mtx_);
        return seek(0L, SEEK_END);
    }

    FileLogger::~FileLogger()
    {
        closeFile();
    }
}