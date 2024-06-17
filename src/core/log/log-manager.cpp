#include "log-manager.hpp"

#include "log-file.hpp"
#include <memory>

namespace CzaraEngine {
    
    Log& derefSharedLog(Shared<Log> &shared_log) {
        return (*(shared_log.get()));
    }

    LogManager::LogManager() {
        if (!getLogMap().contains(m_personal)) {
            LogFileProps personal_log_props;
            personal_log_props.amount = 5000; // 5kB
            personal_log_props.base_directory = "logs";
            personal_log_props.base_name = m_personal + ".txt";
            Shared<SizeLogFile> personal_log{new SizeLogFile(personal_log_props, 4)};
            getLogMap().insert(std::make_pair(m_personal, 
                static_pointer_cast<Log>(personal_log)));
        }
    }
    LogManager::~LogManager() {}
    void LogManager::registerLog(const std::string &log_name, const Shared<Log> &log) {
        getLogMap().insert(std::make_pair(log_name, log));
    }
    std::map<std::string, Shared<Log>>& LogManager::getLogMap() {
        static std::map<std::string, Shared<Log>> m_map;
        return m_map;
    }
    void LogManager::writeToLog(const std::string &log_name, const std::string &msg) {
        if (getLogMap().contains(log_name)) {
            derefSharedLog(getLogMap()[log_name]) << msg << std::endl;
        }
    }
    bool LogManager::hasLog(const std::string &log_name) const {
        return getLogMap().contains(log_name);
    }
}