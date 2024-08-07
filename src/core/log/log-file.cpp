#include "log-file.hpp"
#include <sstream>
#include <cstring>
#include <iostream>


namespace CzaraEngine {
    // LogFile
    LogFile::LogFile(const LogFileProps &props) : Log(), m_amount(props.amount),
        m_base_name(props.base_name), m_base_directory(props.base_directory) {
        try {
            if (!fs::exists(m_base_directory) && !fs::create_directory(m_base_directory)) {
                std::cerr << "Directory was unable to be created.\n";
            }
        } catch (fs::filesystem_error fs_err) {
            std::cerr << "Filesystem error thrown.\n";
        }
    }
    LogFile::LogFile(LogFile * file) : Log(), m_amount(file->m_amount),
        m_base_name(file->m_base_name), m_base_directory(file->m_base_directory) {
        try {
            if (!fs::exists(m_base_directory) && !fs::create_directory(m_base_directory)) {
                std::cerr << "Directory was unable to be created.\n";
            }
        } catch (fs::filesystem_error fs_err) {
            std::cerr << "Filesystem error thrown.\n";
        }
    }
    LogFile::~LogFile() {
        if(m_file.is_open()) {
            m_file.close();
        }
    }
    bool LogFile::isOpen() const {
        return m_file.is_open();
    }
    bool LogFile::close() {
        if (m_file.is_open()) {
            m_file.close();
            return !m_file.is_open();
        }
        return true;
    }

    void LogFile::rotateFile() {
        if (!hasExceededAmount()) return;

        m_file.close();
        m_related_files.push_back(m_active_file);
        m_active_file = generateFileName();
        m_file.open(m_active_file);
    }
    // END LogFile

    // TimeLogFile
    TimeLogFile::TimeLogFile(const LogFileProps &log_file_props) : 
        LogFile(log_file_props) {
        std::chrono::seconds sec = std::chrono::seconds(m_amount);
        if (10min > sec) {
            throw "Invalid file size";
        }
    }
    TimeLogFile::TimeLogFile(TimeLogFile * file) : LogFile(file) {
        std::chrono::seconds sec = std::chrono::seconds(m_amount);
        if (10min > sec) {
            throw "Invalid file size";
        }
    }
    TimeLogFile::~TimeLogFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }
    bool TimeLogFile::open(const fs::path &filename) {
        if(!close()) {
            m_file << "File closure failed.";
            close(); // Attempt to close again.
            throw "File closure failed.  Terminating engine.";
        }
        m_file.open(filename);
        m_active_file = filename;
        return m_file.is_open();
    }
    bool TimeLogFile::hasExceededAmount() {
        chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
        // I'm okay with losing precision, since I wanted time-based file logging to be in seconds.
        auto diff = chrono::duration_cast<std::chrono::seconds>(now - timestamp);
        ui64 diff_sz = diff.count();
        return diff_sz > m_amount;
    }
    fs::path TimeLogFile::generateFileName() {
        std::ostringstream filename;
        chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
        auto dur = chrono::duration_cast<std::chrono::seconds>(now - timestamp);
        timestamp += dur;
        std::time_t t_c = chrono::system_clock::to_time_t(timestamp);
        filename << std::put_time(std::localtime(&t_c), "%F_%H_%M");
        filename << "_" << m_base_name << ".txt";
        return m_base_directory / filename.str();
    }
    std::ostream& TimeLogFile::write(const char * msg) {
        rotateFile();
        return m_file << msg << std::flush;
    }
    std::ostream& TimeLogFile::write(const std::string &msg) {
        rotateFile();
        return m_file << msg << std::flush;
    }
    std::ostream& TimeLogFile::write(std::ostream &msg) {
        rotateFile();
        auto buf = msg.rdbuf();
        return m_file << buf << std::flush;
    }
    // END TimeLogFile

    // SizeLogFile
    SizeLogFile::SizeLogFile(const LogFileProps &log_file_props, ui8 suffix_length) : LogFile(log_file_props),
        m_suffix_length(suffix_length) {
        if (min_bytes_allowed_in_file > m_amount) {
            throw "File specification bytes size is too few for Log File rotation.";
        }
    }
    SizeLogFile::~SizeLogFile() {
        if (m_file.is_open()) {
            m_file.close();
        }
    }
    bool SizeLogFile::open(const fs::path &filename) {
        if(!close()) {
            m_file << "File closure failed.";
            close(); // Attempt to close again.
            throw "File closure failed.  Terminating engine.";
        }
        m_file.open(filename);
        m_active_file = filename;
        return m_file.is_open();
    }
    bool SizeLogFile::hasExceededAmount() {
        ui64 file_size = fs::file_size(m_active_file);
        return (file_size + m_input_buffer) > m_amount;
    }
    fs::path SizeLogFile::generateFileName() {
        std::ostringstream filename;
        filename << m_base_name;
        filename << "_" << std::setfill('0') << std::setw(m_suffix_length) << m_increment << ".txt";
        const_cast<SizeLogFile*>(this)->m_increment++;
        return m_base_directory / filename.str();
    }
    std::ostream& SizeLogFile::write(const char * msg) {
        m_input_buffer = std::strlen(msg);
        rotateFile();
        return m_file << msg << std::flush;
    }
    std::ostream& SizeLogFile::write(const std::string &msg) {
        m_input_buffer = msg.size();
        rotateFile();
        return m_file << msg << std::flush;
    }
    std::ostream& SizeLogFile::write(std::ostream &msg) {
        m_input_buffer = msg.tellp();
        rotateFile();
        return m_file << msg.rdbuf() << std::flush;
    }
    // END SizeLogFile

    // LogFile Friends
    std::string getActiveFileName(LogFile &log_file) {
        return log_file.m_active_file.string();
    }
    bool open(LogFile &log_file) {
        if (!log_file.isOpen()) {
            fs::path filename = log_file.generateFileName();
            return log_file.open(filename);
        }
        return false;
    }

    std::ostringstream LogFile::getClassDetails() const {
        std::ostringstream stream;
        std::string open_status = (isOpen()) ? "OPEN" : "CLOSE";
        stream << "Log File Output Stream:" << endl;
        stream << "\tActive File Name: " << m_active_file << endl;
        stream << "\tOpen Status: " << open_status << " | File Size: " << fs::file_size(m_active_file) << endl;
        stream << "\tBase File Name: " << m_base_name << " | Base Path: " << m_base_directory << endl;
        stream << "\t\tPreviously Created Log Files (" << m_related_files.size() << "):" << endl;
        
        for (const fs::path &prev : m_related_files) {
            stream << "\t\t - " << prev << " | Size: " << fs::file_size(prev) << endl;
        }
        return stream;
    }

    std::ostream& operator<<(LogFile& log_file, std::ostream &stream) {
        if (!log_file.isOpen() && !open(log_file)) {
            std::exit(1);
        }
        return log_file.write(stream);
    }
    std::ostream& operator<<(LogFile& log_file, std::string &str) {
        if (!log_file.isOpen() && !open(log_file)) {
            std::exit(1);
        }
        return log_file.write(str);
    }
    std::ostream& operator<<(LogFile& log_file, const char* c_str) {
        if (!log_file.isOpen() && !open(log_file)) {
            std::exit(1);
        }
        return log_file.write(c_str);
    }
    std::ostream& operator<<(std::ostream& stream, LogFile& log_file) {
        std::ostringstream output = log_file.getClassDetails();
        stream << output.str();
        return stream;
    }

    // End of LogFile
}
