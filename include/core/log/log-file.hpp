#pragma once
#include "log.hpp"

#include "inttypes.hpp"

#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <ostream>

namespace fs = std::filesystem;
namespace chrono = std::chrono;
namespace chrono_lit = std::chrono_literals;
using namespace chrono_lit;

namespace CzaraEngine {
    struct LogFileProps {
        ui64 amount;
        std::string base_name;
        std::string base_directory;
    };

    class LogFile : public Log {
        public:
            virtual ~LogFile();
            virtual bool isOpen() const;
            virtual bool close();
            friend std::string getActiveFileName(LogFile &log_file);
            friend bool open(LogFile &log_file);

            virtual std::ostringstream getClassDetails() const;
            virtual std::ostream& write(const char*) = 0;
            virtual std::ostream& write(const std::string &) = 0;
            virtual std::ostream& write(std::ostream &) = 0;
            friend std::ostream& operator<<(LogFile& log_file, std::ostream& stream);
            friend std::ostream& operator<<(LogFile& log_file, std::string& str);
            friend std::ostream& operator<<(LogFile& log_file, const char* c_str);
            friend std::ostream& operator<<(std::ostream& stream, LogFile& log_file);

        protected:
            LogFile(const LogFileProps &props);
            LogFile(LogFile * file);
            std::ofstream m_file;
            fs::path m_active_file;
            fs::path m_base_directory;
            std::string m_base_name;
            ui64 m_amount = 0;
            std::vector<fs::path> m_related_files;
            virtual void rotateFile();
        private:
            virtual bool open(const fs::path &filename) = 0;
            virtual bool hasExceededAmount() = 0;
            virtual fs::path generateFileName() = 0;
    };

    class TimeLogFile : public LogFile {
        public:
            TimeLogFile(const LogFileProps &log_file_props);
            TimeLogFile(TimeLogFile * file);
            virtual ~TimeLogFile();
            virtual std::ostream& write(const char* msg);
            virtual std::ostream& write(const std::string &msg);
            virtual std::ostream& write(std::ostream &msg);
        private:
            chrono::time_point<chrono::system_clock, chrono::seconds> timestamp;
            //static chrono::minutes min_time_allowed_in_minutes = 10min; // 10min
        private:
            virtual bool open(const fs::path &filename);
            virtual fs::path generateFileName();
            virtual bool hasExceededAmount();
    };

    class SizeLogFile : public LogFile {
        public:
            SizeLogFile(const LogFileProps &log_file_props, ui8 suffix_length);
            SizeLogFile(SizeLogFile * file);
            virtual ~SizeLogFile();
            virtual std::ostream& write(const char* msg);
            virtual std::ostream& write(const std::string &msg);
            virtual std::ostream& write(std::ostream &msg);
        private:
            ui8 m_suffix_length;
            ui64 m_increment = 1;
            ui64 m_input_buffer = 0;
            const static ui64 min_bytes_allowed_in_file = 5000; // 5kB
        private:
            virtual bool open(const fs::path &filename);
            virtual fs::path generateFileName();
            virtual bool hasExceededAmount();
    };
    
    #ifdef _WIN32
        const std::string endl = "\n"; // I was originally using `\r\n` here, but it turns out ofstream auto converts usages of `\n` to `\r\n` on windows.
    #elif defined macintosh // OS 9
        const std::string endl = "\r";
    #else
        const std::string endl = "\n"; // Mac OS X uses \n
    #endif
}
