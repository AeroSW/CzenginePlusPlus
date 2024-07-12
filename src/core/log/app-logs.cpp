#include "app-logs.hpp"
#include "log-utils.hpp"
#include <assert.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace CzaraEngine {
    std::unique_ptr<LogFile> Logger::m_app_log = nullptr;
    std::unique_ptr<LogFile> Logger::m_err_log = nullptr;
    std::unique_ptr<LogFile> Logger::m_file_log = nullptr;
    bool Logger::createApplicationLog(LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_app_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_app_log = createLogFile(props, config.type);
            open(*m_app_log);
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "|| Czengine Application Log ||" << endl;
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    bool Logger::createApplicationLog(const LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_app_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_app_log = createLogFile(props, config.type);
            open(*m_app_log);
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "|| Czengine Application Log ||" << endl;
            *m_app_log << "++--------------------------++" << endl;
            *m_app_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    LogFile& Logger::app_log() {
        std::lock_guard<std::mutex> guard(getMutex());
        if (m_app_log) {
            return *m_app_log;
        }
        throw "Application Log is not instantiated.";
    }
    
    bool Logger::createErrorLog(LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_err_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_err_log = createLogFile(props, config.type);
            open(*m_err_log);
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "||    Czengine Error Log    ||" << endl;
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    bool Logger::createErrorLog(const LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_err_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_err_log = createLogFile(props, config.type);
            open(*m_err_log);
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "||    Czengine Error Log    ||" << endl;
            *m_err_log << "++--------------------------++" << endl;
            *m_err_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    LogFile& Logger::err_log() {
        std::lock_guard<std::mutex> guard(getMutex());
        if (m_err_log) {
            return *m_err_log;
        }
        throw "Error Log is not instantiated.";
    }

    bool Logger::createFileLog(LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_file_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_file_log = createLogFile(props, config.type);
            open(*m_file_log);
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "||    Czengine *File Log    ||" << endl;
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    bool Logger::createFileLog(const LogFileConfig& config) {
        std::lock_guard<std::mutex> guard(getMutex());
        ui64 amount = convertUsableSizes(config.constraints.amount, config.constraints.unit, config.type);
        if (!m_file_log) {
            LogFileProps props = {
                amount,
                config.name,
                config.directory
            };
            m_file_log = createLogFile(props, config.type);
            open(*m_file_log);
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "||    Czengine *File Log    ||" << endl;
            *m_file_log << "++--------------------------++" << endl;
            *m_file_log << "++--------------------------++" << endl;
            return true;
        }
        return false;
    }
    LogFile& Logger::file_log() {
        std::lock_guard<std::mutex> guard(getMutex());
        if (m_file_log) {
            return *m_file_log;
        }
        throw "File Log is not instantiated.";
    }

    std::unique_ptr<LogFile> Logger::createLogFile(LogFileProps& props, const std::string &type) {
        if (type == "size") {
            return std::make_unique<SizeLogFile>(props, 4);
        } else if (type == "time") {
            return std::make_unique<TimeLogFile>(props);
        }
        throw "Invalid Log File Type";
    }
    ui64 Logger::convertUsableSizes(ui64 &amount, const std::string &unit, const std::string &log_type) {
        std::map<std::string, ui64> m_map;
        if (log_type == "time") {
            m_map = getSecondsSizeMap();
        } else if (log_type == "size") {
            m_map = getByteSizeMap();
        }
        auto iter = m_map.find(unit);
        assert(iter != m_map.end() && "Units provided are invalid for this application.");
        ui64 conversion_amount = iter->second;
        return amount * conversion_amount;
    }
    ui64 Logger::convertUsableSizes(const ui64 &amount, const std::string &unit, const std::string &log_type) {
        std::map<std::string, ui64> m_map;
        if (log_type == "time") {
            m_map = getSecondsSizeMap();
        } else if (log_type == "size") {
            m_map = getByteSizeMap();
        }
        auto iter = m_map.find(unit);
        assert(iter != m_map.end() && "Units provided are invalid for this application.");
        ui64 conversion_amount = iter->second;
        return amount * conversion_amount;
    }

    std::mutex& Logger::getMutex() {
        static std::mutex m_mutex;
        return m_mutex;
    }
}