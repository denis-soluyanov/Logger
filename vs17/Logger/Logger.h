#pragma once

#include "Utility.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include <sstream>

namespace dlog
{
    class Logger final : ConsoleLogger, FileLogger
    {
    private:
        using LoggerPtr = std::unique_ptr<Logger>;

        static LoggerPtr instance_;
        Format           format_;

        template<typename T>
        class FormatOutput final
        {
        private:
            const T& ref_;

        public:
            FormatOutput(const T& obj) : ref_(obj) {}

            friend std::ostringstream& operator<<(std::ostringstream& os, const FormatOutput<T>& arg)
            {
                os << ' ' << arg.ref_;
                return os;
            }
        };

        template<typename... Types>
        std::string formatString(const Types&... args)
        {
            std::ostringstream os;

            if (format_.logTime) {
                os << '[' << getCurrentTime(format_.timezone) << "]:";
            }

            (os << ... << FormatOutput(args)) << '\n';
            return os.str();
        }

        Logger(const std::string& filename, const Format& format) : FileLogger(filename), format_(format) {}

        static void createInstance(const std::string& filename, const Format& format)
        {
            instance_ = LoggerPtr(new Logger(filename, format));
        }

    public:
        static void init(const std::string& filename, const Format& format = Format())
        {
            static std::once_flag isCreated;

            if (!instance_)
                std::call_once(isCreated, createInstance, filename, format);
            else
                throw std::logic_error("Attempt to create instance of Logger twice");
        }

        template<typename... Types>
        static void log(const Types&... args)
        {
            if (!instance_) throw std::runtime_error("Instance of Logger wasn't create");

            std::string record = instance_->formatString(args...);

            if (instance_->format_.printInConsole)
                instance_->print_(record);

            instance_->write_(record);
        }

    private:
        /* Disabled */
        Logger(const Logger &) = delete;
        Logger(Logger &&) = delete;
        Logger& operator=(const Logger &) = delete;
        Logger& operator=(Logger &&) = delete;
    };

    Logger::LoggerPtr Logger::instance_ = nullptr;
}