#pragma once

#include <mutex>

namespace dlog
{
    class FileLogger
    {
    private:
        using LockGuard = std::lock_guard<std::mutex>;

        int         fd_;    // file descriptor
        std::mutex  mtx_;   // mutex for exclusive access to file descriptor

        /* Wrapper function above syscall 'write' */
        int write(const void * buf, size_t len) noexcept;

        /* Wrapper function above syscall 'seek' */
        off_t seek(off_t offset, int origin) noexcept;

    public:
        /* Constructs a new instance of FileLogger without associating with the file */
        FileLogger();

        /* This function close (save) the current file */
        void closeFile() noexcept;

        /*
         * This function opens the file for writing or create new file if it doesn't exists.
         * Function returns 'true' if file was successfully opened.
         * Attempting to open another file while you keep the current file open
         * will close the current file and then open a new one.
        */
        bool openFile(const std::string& filename) noexcept;

        /*
         * This function writes the 'record' into current open file.
         * Function returns amount of written bytes or '-1' if an error occurred.
        */
        int writeInFile(const std::string& record) noexcept;

        /* This function returns size of file in bytes */
        off_t getFileSize() noexcept;

        /* Destroys the object and close the file descriptor */
        ~FileLogger();

    private:
        /* Disabled */
        FileLogger(const FileLogger &)            = delete;
        FileLogger(FileLogger &&)                 = delete;
        FileLogger& operator=(const FileLogger &) = delete;
        FileLogger& operator=(FileLogger &&)      = delete;
    };
}