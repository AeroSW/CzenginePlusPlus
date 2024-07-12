#include "czengine-config-parser.hpp"
#include "log-utils.hpp"
#include "sdl-window.hpp"
#include <assert.h>
#include <memory>
#include <sstream>
#include <vector>

namespace CzaraEngine {
    
    const std::string CzengineConfigManager::m_config_file = "./config/czengine.config.toml";
    
    std::unique_ptr<CzengineAppConfig> CzengineConfigManager::m_config = nullptr;
    
    CzengineConfigManager::CzengineConfigManager() {
        if (!m_config) {
            m_config = std::make_unique<CzengineAppConfig>(CzengineAppConfig{});
            parseApplicationConfig();
        }
    }
    CzengineConfigManager::~CzengineConfigManager() {
    }

    const CzengineAppConfig& CzengineConfigManager::getAppConfig() {
        if (m_config) {
            return *m_config;
        }
        m_config = std::make_unique<CzengineAppConfig>(CzengineAppConfig{});
        parseApplicationConfig();
        return *m_config;
    }

    void CzengineConfigManager::parseApplicationConfig() {
        try {
            m_table = toml::parse_file(m_config_file);
        } catch (const toml::parse_error &err) {
            std::ostringstream err_out;
            err_out << "Failed to parse application config.\n" << err << std::endl;
            SdlWindow::showErrorMessageBox("Czengine Config Alert", err_out.str().c_str());
            std::exit(1);
        }
        std::map<std::string, LogFileConfig> log_file_configs = processLogFileConfigs();
        if (log_file_configs.size() != 3) {
            std::ostringstream err_out;
            err_out << "Malformed log list.  Should have 3 Log Files, [application], [error], and [file].\nSize: " << log_file_configs.size() << std::endl;
            SdlWindow::showErrorMessageBox("Czengine Config Alert", err_out.str().c_str());
            std::exit(1);
        }
        m_config->title = m_table["application"]["title"].value_or("Czengine++");
        m_config->author = m_table["application"]["author"].value_or("AeroCzara");
        m_config->interface = processUxFileConfig();
        m_config->application_log = log_file_configs["application"];
        m_config->error_log = log_file_configs["error"];
        m_config->file_log = log_file_configs["file"];
    }

    FileConfig CzengineConfigManager::processUxFileConfig() {
        std::optional<std::string> dir = m_table["interface"]["directory"].value<std::string>();
        std::optional<std::string> name = m_table["interface"]["name"].value<std::string>();
        assert((name.has_value() && dir.has_value()) && "Both directory and name require a value for parsing UX related XML file.");
        return FileConfig{name.value(), dir.value()};
    }

    std::map<std::string, LogFileConfig> CzengineConfigManager::processLogFileConfigs() {
        std::optional<std::string> dir = m_table["logging"]["directory"].value<std::string>();
        std::map<std::string, LogFileConfig> log_configs;
        if (toml::array *logs_array = m_table["logging"]["logs"].as_array()) {
            for (toml::node& log_node : *logs_array) {
                if (!log_node.is_table()) {
                    SdlWindow::showErrorMessageBox("Czengine Config Alert", "Malformed log config in TOML.  It should be a [table].");
                    std::exit(1);
                }
                toml::table& log = *log_node.as_table();
                std::optional<std::string> log_name = log["name"].value<std::string>();
                std::optional<std::string> log_type = log["type"].value<std::string>();
                std::optional<ui64> log_iteration_amount = log["constraints"]["amount"].value<ui64>();
                std::optional<std::string> log_iteration_unit = log["constraints"]["unit"].value<std::string>();
                bool name_result = validLogFileName(log_name.value_or(""));
                bool type_result = validLogFileType(log_type.value_or(""));
                bool unit_result = validLogFileUnit(log_type.value_or(""), log_iteration_unit.value_or(""));
                assert((name_result && type_result && unit_result) && "Invalid Log Name and Type");
                log_configs[log_name.value()] = LogFileConfig {
                    log_name.value(),
                    dir.value(),
                    log_type.value(), {
                        log_iteration_amount.value(),
                        log_iteration_unit.value()
                    }
                };
            }
        }
        return log_configs;
    }

    bool CzengineConfigManager::validLogFileName(const std::string &log_name) {
        static std::vector<std::string> m_log_names = { "application", "error", "file" };
        for (size_t ii = 0; ii < m_log_names.size(); ii++) {
            if (m_log_names[ii] == log_name) return true;
        }
        return false;
    }
    bool CzengineConfigManager::validLogFileType(const std::string &log_type) {
        static std::vector<std::string> m_log_types = { "size", "time" };
        for (size_t ii = 0; ii < m_log_types.size(); ii++) {
            if (m_log_types[ii] == log_type) return true;
        }
        return false;
    }
    bool CzengineConfigManager::validLogFileUnit(const std::string &log_type, const std::string &log_unit) {
        if (log_type == "size") {
            auto byte_iter = getByteSizeMap().find(log_unit);
            return byte_iter != getByteSizeMap().end();
        } else if (log_type == "time") {
            auto time_iter = getSecondsSizeMap().find(log_unit);
            return time_iter != getSecondsSizeMap().end();
        }
        return false;
    }
}
