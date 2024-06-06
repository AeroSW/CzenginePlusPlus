#include <memory>
#include <filesystem>
#include <fstream>
#include "window.hpp"
#include "sdl-window.hpp"
#include "log-file.hpp"
#include "app-configuration.hpp"

namespace fs = std::filesystem;
using namespace CzaraEngine;

int main() {
    std::cout << "Welcome to the Czengine Project\n";
    //Shared<Window> window;
    if(!app_config.initReference<AppState>(AppState::__DEBUG__)) {
        std::cerr << "Issue initializing Application Configuration." << std::endl;
        return 1;
    }
    LogFileProps app_log_props;
    fs::path current = fs::current_path();
    app_log_props.amount = 3600;
    app_log_props.base_directory = app_config.getReference().default_log_dir;
    app_log_props.base_name = "application.txt";
    
    Shared<LogFile> application_log{new TimeLogFile(app_log_props)};
    open(*application_log);
    (*(application_log.get())) << "Test Log 1";
    try {
        WindowProperties props;
        props.height = 720;
        props.width = 1080;
        props.x_window_offset = SdlWindow::X_CENTER;
        props.y_window_offset = SdlWindow::Y_CENTER;
        props.name = "AeroCzengine";
        Shared<Window> window{new SdlWindow(props)};
    } catch (std::string err_msg) {
        //std::ostringstream sample;
        //sample << "Error found Opening window: " << err_msg;
        //(*(application_log.get())) << "Error found opening window" << err_msg;
        //(*application_log) << sample;
        //return -1;
    } catch(...) {
        std::cout << "Caught the error somehow.\n";
    }
    std::cout << "LLLLLL\n";
    std::cout << (*application_log) << std::endl;
    return 0;
}
