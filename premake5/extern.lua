includes = {
    "./include/**",
    "./extern/__include__/DearImGui/",
    "./extern/__include__/DearImGui/**",
    "./extern/__include__/PugiXml/src",
    "./extern/__include__/Toml++/include",
    "./extern/__include__/Toml++/include/**",
    "./extern/__vendor__/SDL2/include/**",
    "./extern/__vendor__/SDL2/include/SDL2/**",
    "./extern/__vendor__/SDL2_ttf/include/**",
    "./extern/__vendor__/SDL2_ttf/include/SDL2/**",
    "./extern/__vendor__/Vulkan/Include",
    "./extern/__include__/GoogleTest/googletest",
    "./extern/__include__/GoogleTest/googlemock",
    "./extern/__include__/GoogleTest/googletest/include",
    "./extern/__include__/GoogleTest/googlemock/include",
    "./extern/__include__/JoMock/jomock"
}
libs = {
    "./extern/__vendor__/Vulkan/Bin",
    "./extern/__vendor__/SDL2/bin",
    "./extern/__vendor__/SDL2_ttf/bin"
}
slinks = {
    "./extern/__vendor__/Vulkan/Lib/vulkan-1",
    "./extern/__vendor__/SDL2/lib/libSDL2",
    "./extern/__vendor__/SDL2_ttf/lib/libSDL2_ttf"
}
excludes = {
    "extern/__vendor__/**/*.cpp", 
    "extern/__vendor__/**/*.c",
    "extern/__include__/JoMock/example/*.cpp",
    "extern/__include__/JoMock/example/**/*.cpp",
    "extern/__include__/JoMock/vs2019_example/*.cpp",
    "extern/__include__/JoMock/vs2019_example/**/*.cpp",
    "extern/__include__/GoogleTest/googletest/test/*.cc",
    "extern/__include__/GoogleTest/googlemock/test/*.cc",
    "extern/__include__/GoogleTest/googletest/test/**/*.cc",
    "extern/__include__/GoogleTest/googlemock/test/**/*.cc",
    "extern/__include__/GoogleTest/googletest/samples/*.cc",
    "extern/__include__/GoogleTest/googlemock/samples/*.cc",
    "extern/__include__/GoogleTest/googletest/samples/**/*.cc",
    "extern/__include__/GoogleTest/googlemock/samples/**/*.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock_main.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock-cardinalities.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock-internal-utils.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock-matchers.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock-spec-builders.cc",
    "extern/__include__/GoogleTest/googlemock/src/gmock.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest_main.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-assertion-result.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-death-test.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-filepath.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-matchers.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-port.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-printers.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-test-part.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest-typed-test.cc",
    "extern/__include__/GoogleTest/googletest/src/gtest.cc",
    "extern/__include__/PugiXml/tests/*.cpp",
    "extern/__include__/PugiXml/docs/**/*.cpp",
    "extern/__include__/DearImGui/examples/**.cpp",
    "extern/__include__/DearImGui/misc/**/*.cpp",
    "extern/__include__/DearImGui/backends/*allegro5.cpp", -- Remove DearImGui Backends not being used.
    "extern/__include__/DearImGui/backends/*android.cpp",
    "extern/__include__/DearImGui/backends/*dx10.cpp",
    "extern/__include__/DearImGui/backends/*dx11.cpp",
    "extern/__include__/DearImGui/backends/*dx12.cpp",
    "extern/__include__/DearImGui/backends/*dx9.cpp",
    "extern/__include__/DearImGui/backends/*glfw.cpp",
    "extern/__include__/DearImGui/backends/*opengl*.cpp",
    "extern/__include__/DearImGui/backends/*glut.cpp",
    "extern/__include__/DearImGui/backends/*metal.cpp",
    "extern/__include__/DearImGui/backends/*osx.h",
    "extern/__include__/DearImGui/backends/*sdl3.cpp",
    "extern/__include__/DearImGui/backends/*sdlrenderer3.cpp",
    "extern/__include__/DearImGui/backends/*wgpu.cpp",
    "extern/__include__/DearImGui/backends/*win32.cpp"
}