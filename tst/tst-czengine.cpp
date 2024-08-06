#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <filesystem>
#include <fcntl.h>

namespace fs = std::filesystem;

/**
 * Configures C's `printf` method to print to `./output/tst-czengine.result.txt`.
 * 
 * Method utilizes C++'s <filesystem> library to ensure the directory "output" exists.
 * It then utilizes C's <fcntl.h> library to open the `tst-czengine.result.txt` file in
 * the output directory and configures the stdout to print into this file.
 * Source: https://stackoverflow.com/a/35186153/7044902
 */
void setup() {
    int fd;
    fs::path dir_path{"./output"};
    fs::path filename = dir_path / "tst-czengine.result.txt";
    if (!fs::is_directory(dir_path) && !fs::create_directory(dir_path)) {
        perror("Failed to find and create `./output`.");
        std::exit(1);
    }
    fd = open(filename.string().c_str(), O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Open failed");
        std::exit(1);
    }
    if (dup2(fd, 1) == -1) {
        perror("dup2 failed");
        std::exit(1);
    }
}

int main(int argc, char **argv) {
    setup();
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    int result = RUN_ALL_TESTS();
    return result;
}