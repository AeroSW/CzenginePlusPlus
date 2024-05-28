
#include <iostream>
#include <memory>
#include "window.hpp"
#include "sdl-window.hpp"
//#include "SDL.h"

int main() {
    std::cout << "Welcome to the Czengine Project\n";
    std::unique_ptr<Window> window;
    try {
        WindowProperties props;
        props.height = 720;
        props.width = 1080;
        props.x_window_offset = SdlWindow::X_CENTER;
        props.y_window_offset = SdlWindow::Y_CENTER;
        props.name = "AeroCzengine";
        window = std::make_unique<SdlWindow>(new SdlWindow(props));

    } catch (std::string err_msg) {
        std::cerr << err_msg << "\n";
        return -1;
    }
    while(window->isOpen()) {
        std::cout << "Window is open\n";
    }
    return 0;
}