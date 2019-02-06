#pragma once

#include <fstream>
#include <mutex>

namespace dlog
{
    class FileLogger
    {
    private:
        std::ofstream   fstream_;
        std::string     filename_;
        std::mutex      mtx_;

    protected:
        explicit FileLogger(const std::string& filename) : filename_(filename)
        {
            fstream_.open(filename_, std::ios::app);
        }

        void write_(const std::string& record)
        {
            std::lock_guard<std::mutex> guard(mtx_);

            fstream_.write(record.c_str(), record.size());
        }

    public:
        ~FileLogger()
        {
            try {
                fstream_.close();
            }
            catch (...) {}
        }

    private:
        /* Disabled */
        FileLogger(const FileLogger &) = delete;
        FileLogger(FileLogger &&) = delete;
        FileLogger& operator=(const FileLogger &) = delete;
        FileLogger& operator=(FileLogger &&) = delete;
    };
}