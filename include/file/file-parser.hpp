#pragma once

#include <filesystem>
#include "app-configuration.hpp"
#include <memory>
#include "log-file.hpp"
#include "exception.hpp"
#include "log-manager.hpp"
#include <vector>

namespace fs = std::filesystem;

namespace CzaraEngine {
    template<typename T>
    class FileParser {
        public:
            virtual ~FileParser();
            virtual std::vector<T> processFile() = 0;
        protected:
            FileParser(const fs::path path);
            fs::path m_path;
            static LogManager& getLogManager();
            static LogManager m_log_manager;
            const static std::string m_log_name;
    };
    template<typename T>
    const std::string FileParser<T>::m_log_name = "Czengine_File_Log";
    template<typename T>
    LogManager& FileParser<T>::getLogManager() {
        static LogManager m_log_manager;
        return m_log_manager;
    }
    template<typename T>
    FileParser<T>::FileParser(const fs::path path) : m_path(path) {
        std::shared_ptr<SizeLogFile> log{new SizeLogFile({10000, m_log_name, app_config.getReference().default_log_dir}, 4)};
        if (!getLogManager().hasLog(m_log_name)) {
            getLogManager().registerLog(m_log_name, static_pointer_cast<Log>(log));
            open(*(log.get()));
        }
    }
    template<typename T>
    FileParser<T>::~FileParser() {}
}
