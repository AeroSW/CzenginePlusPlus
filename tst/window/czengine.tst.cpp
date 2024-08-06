#include "gtest/gtest.h"
#include "czengine.hpp"
#include "base_class.mock.hpp"
#include "sdl-renderer-binding-factory.hpp"
#include <string>
#include <memory>
#include "jomock.h"
using namespace ::testing;
namespace CzaraEngine::Test {
    class CzengineFixture : public testing::Test {
        public:
            enum Configurations {
                VULKAN,
                SDL2,
                BREAK
            };
            CzengineFixture() {
                setupAppConfig(vulkan_config, CzengineFixture::Configurations::VULKAN);
                setupAppConfig(sdl2_config, CzengineFixture::Configurations::SDL2);
                setupAppConfig(break_config, CzengineFixture::Configurations::BREAK);
            }
            void SetUp() override {
                CLEAR_JOMOCK();
                SDL_Init(SDL_INIT_VIDEO);
                test_window = SDL_CreateWindow(
                    sdl2_config.title.c_str(),
                    sdl2_config.window.x_window_offset,
                    sdl2_config.window.y_window_offset,
                    sdl2_config.window.width,
                    sdl2_config.window.height,
                    (SDL_WindowFlags) (SDL_WINDOW_RESIZABLE)
                );
                test_renderer = new MockRenderer();
            }
            void TearDown() override {
                CLEAR_JOMOCK();
                if (test_window) {
                    std::cout << "Tear Down()'s SDL_DestroyWindow" << std::endl;
                    SDL_DestroyWindow(test_window);
                    test_window = nullptr;
                }
                if (test_renderer != nullptr) {
                    delete test_renderer;
                    test_renderer = nullptr;
                }
                SDL_Quit();
            }
        protected:
            CzengineAppConfig vulkan_config;
            CzengineAppConfig sdl2_config;
            CzengineAppConfig break_config;
            void setupAppConfig(CzengineAppConfig &config, const CzengineFixture::Configurations &custom) {
                config.title = "CzengineTest";
                config.window.width = 100;
                config.window.height = 75;
                config.window.x_window_offset = 0;
                config.window.y_window_offset = 0;
                config.render.device = "";
                if (custom == CzengineFixture::Configurations::VULKAN) {
                    config.render.api = "Vulkan";
                } else if (custom == CzengineFixture::Configurations::SDL2) {
                    config.render.api = "SDL2";
                } else if (custom == CzengineFixture::Configurations::BREAK) {
                    config.render.api = "";
                }
            }
            SDL_Window * test_window;
            MockRenderer * test_renderer;
    };
    typedef std::shared_ptr<SDL_Window> shared_window;
    TEST_F(CzengineFixture, testDefaultInstantiation) {
        // auto sdl_init_mock = &JOMOCK(SDL_Init);
        // auto sdl_createwindow_mock = &JOMOCK(SDL_CreateWindow);
        // auto sdl_destroywindow_mock = &JOMOCK(SDL_DestroyWindow);
        // EXPECT_CALL(*sdl_init_mock, JOMOCK_FUNC(_))
        //     .Times(Exactly(1))
        //     .WillOnce(Return(0));
        // EXPECT_CALL(*sdl_createwindow_mock, JOMOCK_FUNC(_, _, _, _, _, _))
        //     .Times(Exactly(1))
        //     .WillOnce(Return(test_window));
        // EXPECT_CALL(*sdl_destroywindow_mock, JOMOCK_FUNC(_))
        //     .Times(Exactly(1))
        //     .WillRepeatedly(Return());
        { // Scoping Czengine instantiation to also allow destructor to be called.
            Czengine czengine{sdl2_config};
            std::cout << "Destroy Window Call in test" << std::endl;
        }
        std::cout << "Czengine out of scope." << std::endl;
        ASSERT_TRUE(true); // If we made it here, then we succeeded.
    }
}