#pragma once

#include "Utility.h"
#include "ConsoleLogger.h"
#include "DefaultLogger.h"

class Logger final : private ConsoleLogger, private DefaultLogger
{
private:
    using LoggerPtr = std::unique_ptr<Logger>;

    static LoggerPtr instance_;

    Logger(const std::string& filename, std::ostream& os = std::clog) : ConsoleLogger(os), DefaultLogger(filename)
    {
    }

    static void createInstance(const std::string& filename, std::ostream& os)
    {
        instance_ = LoggerPtr(new Logger(filename, os));
    }

public:
    static void init(const std::string& filename, std::ostream& os = std::clog)
    {
        static std::once_flag isCreated;

        if (!instance_)
            std::call_once(isCreated, createInstance, filename, os);
        else
            throw std::logic_error("Attempt to create instance of Logger twice");
    }

    template<typename... Types>
    static void log(const Types&... args)
    {
        instance_->print_(args...);
        instance_->write_(args...);
    }

private:
    /* Disabled */
    Logger(const Logger &)            = delete;
    Logger(Logger &&)                 = delete;
    Logger& operator=(const Logger &) = delete;
    Logger& operator=(Logger &&)      = delete;
};

Logger::LoggerPtr Logger::instance_ = nullptr;