#pragma once

#include "Utility.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include <sstream>
#include <memory>

namespace dlog
{
    class Logger final : ConsoleLogger, FileLogger
    {
    private:
        using LoggerPtr = std::unique_ptr<Logger>;

        static LoggerPtr instance_;
        Adapter          adapter_;

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

            if (adapter_.logTime) {
                os << '[' << getCurrentTime(adapter_.time) << "]:";
            }

            (os << ... << FormatOutput(args)) << '\n';
            return os.str();
        }

        Logger(const std::string& filename, const Adapter& adapter);

        static void createInstance(const std::string& filename, const Adapter& adapter);

        template<typename Type>
        friend void init(const std::string& filename, const Adapter& adapter);

        bool canWrite() noexcept;

    public:

        template<typename... Types>
        static void log(const Types&... args)
        {
            if (!instance_) throw std::runtime_error("Instance of Logger wasn't create");

            if (instance_->canWrite()) {
                instance_->print_("Maximum file size reached!\n");
                return;
            }

            std::string record = instance_->formatString(args...);

            if (instance_->adapter_.printInConsole)
                instance_->print_(record);

            instance_->writeInFile(record);
        }

    private:
        /* Disabled */
        Logger(const Logger &)            = delete;
        Logger(Logger &&)                 = delete;
        Logger& operator=(const Logger &) = delete;
        Logger& operator=(Logger &&)      = delete;
    };

    template<typename Type = Adapter>
    void init(const std::string& filename, const Adapter& adapter = Adapter())
    {
        static std::once_flag isCreated;

        if (!Logger::instance_)
            std::call_once(isCreated, Logger::createInstance, filename, adapter);
        else
            throw std::logic_error("Attempt to create instance of Logger twice");
    }
}