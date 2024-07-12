#pragma once

#define TOML_ENABLE_PARSER 1

#ifdef _WIN32
    #define TOML_ENABLE_WINDOWS_COMPAT 1
#endif

#include <memory>
#include <string>
#include "czengine-app-config.hpp"
#define TOML_HEADER_ONLY 1
#include "toml.hpp"

namespace CzaraEngine {
    struct AppConfiguration {};
    class CzengineConfigManager {
        public:
            CzengineConfigManager();
            virtual ~CzengineConfigManager();

            const CzengineAppConfig& getAppConfig();

        private:
            const static std::string m_config_file;
            static std::unique_ptr<CzengineAppConfig> m_config;
            toml::table m_table;

        private:
            void parseApplicationConfig();
            std::map<std::string, LogFileConfig> processLogFileConfigs();
            FileConfig processUxFileConfig();
            bool validLogFileName(const std::string &log_name);
            bool validLogFileType(const std::string &log_type);
            bool validLogFileUnit(const std::string &log_type, const std::string &log_unit);
    };
}