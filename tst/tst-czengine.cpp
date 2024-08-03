#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <filesystem>
#include <fstream>
#include <fcntl.h>

namespace fs = std::filesystem;

// struct OutputBuffers {
//     std::streambuf * original_cout_buffer;
//     std::ofstream output_file;
// } output_config;

void setup() {
    int fd;
    const char * filename = "./output/tst-czengine.result.txt";
    fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Open failed");
        std::exit(1);
    }
    if (dup2(fd, 1) == -1) {
        perror("dup2 failed");
        std::exit(1);
    }
}

// void teardown() {
//     if (output_config.output_file.is_open()) {
//         output_config.output_file.close();
//     }
//     std::cout.rdbuf(output_config.original_cout_buffer);
// }

int main(int argc, char **argv) {
    setup();
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    int result = RUN_ALL_TESTS();
    // teardown();
    return result;
}