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
        using LockGuard = std::lock_guard<std::mutex>;

        int write(const void * buf, size_t len) noexcept
        {
            if (fd_ < 0) return -1;
#ifdef _WIN32
            return ::_write(fd_, buf, static_cast<unsigned int>(len));
#else
            return ::write(fd_, buf, len);
#endif
        }

        off_t seek(off_t offset, int origin) noexcept
        {
            if (fd_ < 0) return -1;
#ifdef _WIN32
            return ::_lseek(fd_, offset, origin);
#else
            return ::lseek(fd_, offset, origin);
#endif
        }

    public:
        /* Constructs a new instance of FileLogger without associating with the file */
        FileLogger() : fd_(-1) {}

        /* This function close (save) current file */
        void closeFile() noexcept
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

        /*
         * This function opens the file for writing or create new file if it doesn't exists.
         * Function returns 'true' if file was successfully opened.
         * Attempting to open another file while you keep the current file open
         * will close the current file and then open a new one.
        */
        bool openFile(const std::string& filename) noexcept
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

        /*
         * This function writes the 'record' into current open file.
         * Function returns amount of written bytes or '-1' if an error occurred.
        */
        int writeInFile(const std::string& record) noexcept
        {
            LockGuard guard(mtx_);
            return write(record.c_str(), record.size());
        }

        /* This function returns size of file in bytes */
        off_t getFileSize() noexcept
        {
            LockGuard guard(mtx_);
            return seek(0L, SEEK_END);
        }

        /* Destroys the object and close the file descriptor */
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