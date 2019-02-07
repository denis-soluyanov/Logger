#pragma once

#include <mutex>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace dlog
{
    class FileLogger
    {
    private:
        int         fd_;    // file descriptor
        std::mutex  mtx_;   // mutex for exclusive access to file descriptor

        void openFile(const std::string& filename) noexcept
        {
#ifdef _WIN32
            ::_sopen_s(&fd_, filename.c_str(), _O_CREAT | _O_WRONLY, _SH_DENYWR, _S_IREAD | _S_IWRITE);
#else
            fd_ = ::open(filename.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
        }

        int writeFile(const void* buf, size_t len) noexcept
        {
            if (fd_ < 0) return fd_;
#ifdef _WIN32
            return ::_write(fd_, buf, static_cast<unsigned int>(len));
#else
            return ::write(fd_, buf, len);
#endif
        }

        void closeFile() noexcept
        {
            if (fd_ < 0) {
#ifdef _WIN32
                ::_close(fd_);
#else
                ::close(fd_);
#endif
                fd_ = -1;
            }
        }

    protected:
        explicit FileLogger(const std::string& filename) : fd_(-1)
        {
            openFile(filename);
        }

        void write_(const std::string& record)
        {
            std::lock_guard<std::mutex> guard(mtx_);

            writeFile(record.c_str(), record.size());
        }

    public:
        ~FileLogger()
        {
            closeFile();
        }

    private:
        /* Disabled */
        FileLogger(const FileLogger &)            = delete;
        FileLogger(FileLogger &&)                 = delete;
        FileLogger& operator=(const FileLogger &) = delete;
        FileLogger& operator=(FileLogger &&)      = delete;
    };
}