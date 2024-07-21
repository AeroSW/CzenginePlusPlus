#pragma once

#include "sdl-renderer-binding-factory.hpp"

#ifndef BIND_RENDERER
    #define BIND_RENDERER(NAME, TYPE, ...) \
        namespace { \
            ::CzaraEngine::SdlRendererBindingFactory::Registrar<TYPE, ##__VA_ARGS__> registrar__##NAME(#NAME); \
        }
#endif
