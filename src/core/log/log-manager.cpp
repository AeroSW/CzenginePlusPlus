#include "log-manager.hpp"

#include "log-file.hpp"

namespace CzaraEngine {
    std::map<std::string, Shared<Log>> LogManager::logs;
    Log& derefSharedLog(Shared<Log> &shared_log) {
        return (*(shared_log.get()));
    }

    LogManager::LogManager() {
        if (!logs.contains(m_personal)) {
            LogFileProps personal_log_props;
            personal_log_props.amount = 5000; // 5kB
            personal_log_props.base_directory = "logs";
            personal_log_props.base_name = m_personal + ".txt";
            Shared<SizeLogFile> personal_log{new SizeLogFile(personal_log_props, 4)};
            logs.insert(std::make_pair(m_personal, 
                static_pointer_cast<Log>(personal_log)));
        }
    }
    LogManager::~LogManager() {}
    void LogManager::registerLog(const std::string &log_name, const Shared<Log> &log) {
        logs.insert(std::make_pair(log_name, log));
    }
    void LogManager::writeToLog(const std::string &log_name, const std::string &msg) {
        if (logs.contains(log_name)) {
            derefSharedLog(logs[log_name]) << msg << std::endl;
        }
    }
    bool LogManager::hasLog(const std::string &log_name) const {
        return logs.contains(log_name);
    }
}