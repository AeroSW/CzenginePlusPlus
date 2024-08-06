#pragma once
#include "gmock/gmock.h"

#include "inttypes.hpp"
#include "interface.hpp"
#include "renderer.hpp"

#include "sdl-renderer-binding-factory.hpp"
#include "xml-interface-binding-factory.hpp"
#include "SDL.h"
#include <functional>
#include <memory>
#include <string>

namespace CzaraEngine::Test {
    class MockRenderer : public Renderer {
        public:
            MOCK_METHOD(void, drawColorRgb, (const ui8 &red, const ui8 &green, const ui8 &blue, const ui8 &alpha), (override));
            MOCK_METHOD(void, clearRenderBuffer, (), (override));
            MOCK_METHOD(void, render, (), (override));

    };

    class MockInterface : public CzengineInterface {
        public:
            MOCK_METHOD(void, addComponent, (std::shared_ptr<Component>&), (override));
            MOCK_METHOD(void, addComponents, (std::vector<std::shared_ptr<Component>>&), (override));
            MOCK_METHOD(void, newFrame, (), (override));
            MOCK_METHOD(void, render, (), (override));
            MOCK_METHOD(void, draw, (), (override));
            MOCK_METHOD(void, drawInterface, (), (override));
            MOCK_METHOD(void, processEvent, (const SDL_Event&), (override));
    };
}
