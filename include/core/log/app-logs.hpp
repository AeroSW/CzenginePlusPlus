#pragma once

#include "czengine-app-config.hpp"
#include "log-file.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace CzaraEngine {
    class Logger {
        public:
            static bool createApplicationLog(LogFileConfig& config);
            static bool createErrorLog(LogFileConfig& config);
            static bool createFileLog(LogFileConfig& config);
            static bool createApplicationLog(const LogFileConfig& config);
            static bool createErrorLog(const LogFileConfig& config);
            static bool createFileLog(const LogFileConfig& config);

            static LogFile& app_log();
            static LogFile& err_log();
            static LogFile& file_log();

        private:
            static std::unique_ptr<LogFile> m_app_log;
            static std::unique_ptr<LogFile> m_err_log;
            static std::unique_ptr<LogFile> m_file_log;

        private:
            static std::unique_ptr<LogFile> createLogFile(LogFileProps& props, const std::string &type);
            static ui64 convertUsableSizes(ui64 &amount, const std::string &unit, const std::string &log_type);
            static ui64 convertUsableSizes(const ui64 &amount, const std::string &unit, const std::string &log_type);
            static std::mutex& getMutex();
    };
};